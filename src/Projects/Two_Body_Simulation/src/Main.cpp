#include "Main.h"
#include "Core/Common/Common.h"
#include "Core/Context.h"

#include "Core/Common/pch.h"

bool Two_Body_Simulation::Init()
{
    hasUI = true;

    // pipeline creation
    SDL_GPUShader *vertShader =
        Common::LoadShader(gContext.renderData.device, "DrawCircle.vert", 0, 1, 1, 0);
    SDL_GPUShader *fragShader =
        Common::LoadShader(gContext.renderData.device, "DrawCircle.frag", 0, 0, 0, 0);

    const SDL_GPUColorTargetBlendState blendState {
        .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
        .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        .color_blend_op        = SDL_GPU_BLENDOP_ADD,
        .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
        .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
        .enable_blend          = true,
    };

    const SDL_GPUColorTargetDescription colorDesc {
        .format      = SDL_GetGPUSwapchainTextureFormat(gContext.renderData.device,
                                                        gContext.renderData.window),
        .blend_state = blendState,
    };

    const SDL_GPUGraphicsPipelineTargetInfo targetInfo {
        .color_target_descriptions = &colorDesc,
        .num_color_targets         = 1,
    };

    const SDL_GPUMultisampleState mState = {
        .sample_count = gContext.renderData.sampleCount,  // MSAA or not
    };

    const SDL_GPUGraphicsPipelineCreateInfo createInfo {
        .vertex_shader     = vertShader,
        .fragment_shader   = fragShader,
        .primitive_type    = SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP,
        .multisample_state = mState,
        .target_info       = targetInfo,
    };

    renderPipeline = SDL_CreateGPUGraphicsPipeline(gContext.renderData.device, &createInfo);

    SDL_ReleaseGPUShader(gContext.renderData.device, vertShader);
    SDL_ReleaseGPUShader(gContext.renderData.device, fragShader);

    const SDL_GPUTransferBufferCreateInfo tBufCreateInfo {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size  = (Uint32)(Particles.count * sizeof(ParticleDataSend)),
    };

    transferBuffer = SDL_CreateGPUTransferBuffer(gContext.renderData.device, &tBufCreateInfo);

    const SDL_GPUBufferCreateInfo newBufCreateInfo {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size  = (Uint32)(Particles.count * sizeof(ParticleDataSend)),

    };

    dataBuffer = SDL_CreateGPUBuffer(gContext.renderData.device, &newBufCreateInfo);

    Particles.init();

    return true;
}

bool Two_Body_Simulation::Update()
{
    Particles.update();
    return true;
}

bool Two_Body_Simulation::Draw()
{
    const float w = gContext.appState.ProjectWindowX;
    const float h = gContext.appState.ProjectWindowY;

    const glm::mat4x4 projection      = glm::ortho(0.0f, w, h, 0.0f, 0.0f, -1.0f);
    const glm::mat4x4 finalProjection = projection;
    // const glm::mat4x4 offset     = glm::translate(glm::mat4(1.0f), glm::vec3(w / 2, h / 2,
    // 0.0f)); const glm::mat4x4 finalProjection = projection * offset;

    SDL_GPUCommandBuffer *cmdBuf = SDL_AcquireGPUCommandBuffer(gContext.renderData.device);
    if (cmdBuf == nullptr) { std::cerr << "unable to aquire command buffer\n"; }

    if (gContext.renderData.projectTexture)
    {
        // upload the data to transfer buffer
        auto *dataPtr = static_cast<ParticleDataSend *>(
            SDL_MapGPUTransferBuffer(gContext.renderData.device, transferBuffer, true));

        for (int i = 0; i < Particles.count; i++)
        {
            dataPtr[i].Color    = Particles.ParticleVec[i].Color;
            dataPtr[i].Position = Particles.ParticleVec[i].Position;
            dataPtr[i].Radius   = Particles.ParticleVec[i].Radius;
            dataPtr[i]._padding = 0.0f;
        }

        SDL_UnmapGPUTransferBuffer(gContext.renderData.device, transferBuffer);

        // upload the data to gpu from transfer buffers
        const SDL_GPUTransferBufferLocation transferBufferLocation {
            .transfer_buffer = transferBuffer,
            .offset          = 0,
        };

        const SDL_GPUBufferRegion bufferRegion {
            .buffer = dataBuffer,
            .offset = 0,
            .size   = (Uint32)(Particles.count * sizeof(ParticleDataSend)),
        };

        SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuf);
        SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &bufferRegion, true);
        SDL_EndGPUCopyPass(copyPass);

        SDL_GPUColorTargetInfo tinfo {.texture     = gContext.renderData.projectTexture,
                                      .clear_color = {0.094f, 0.094f, 0.145f, 1.00f},
                                      .load_op     = SDL_GPU_LOADOP_CLEAR,
                                      .store_op    = SDL_GPU_STOREOP_STORE,
                                      .cycle       = false};

        if (gContext.renderData.sampleCount == SDL_GPU_SAMPLECOUNT_1)
        {
            tinfo.store_op = SDL_GPU_STOREOP_STORE;
        }
        else
        {
            tinfo.store_op        = SDL_GPU_STOREOP_RESOLVE;
            tinfo.resolve_texture = gContext.renderData.resolveTexture;
        }

        gContext.renderData.projectPass = SDL_BeginGPURenderPass(cmdBuf, &tinfo, 1, nullptr);

        SDL_BindGPUGraphicsPipeline(gContext.renderData.projectPass, renderPipeline);
        SDL_BindGPUVertexStorageBuffers(gContext.renderData.projectPass, 0, &dataBuffer, 1);

        SDL_PushGPUVertexUniformData(cmdBuf, 0, &finalProjection, sizeof(glm::mat4x4));

        SDL_DrawGPUPrimitives(gContext.renderData.projectPass, 4, Particles.count, 0, 0);

        SDL_EndGPURenderPass(gContext.renderData.projectPass);

        SDL_GPUTexture *blitSourceTexture =
            (tinfo.resolve_texture != nullptr) ? tinfo.resolve_texture : tinfo.texture;

        const SDL_GPUBlitRegion blitSrc = {
            .texture = blitSourceTexture,
            .w       = (Uint32)gContext.appState.ProjectWindowX,
            .h       = (Uint32)gContext.appState.ProjectWindowY,
        };

        SDL_BindGPUFragmentStorageTextures(gContext.renderData.projectPass, 1,
                                           &gContext.renderData.projectTexture, 2);

        const SDL_GPUBlitRegion blitDst = {
            .texture = gContext.renderData.projectTexture,
            .w       = (Uint32)gContext.appState.ProjectWindowX,
            .h       = (Uint32)gContext.appState.ProjectWindowY,
        };

        const SDL_GPUBlitInfo blitInfo = {.source      = blitSrc,
                                          .destination = blitDst,
                                          .load_op     = SDL_GPU_LOADOP_DONT_CARE,
                                          .filter      = SDL_GPU_FILTER_LINEAR};

        SDL_BlitGPUTexture(cmdBuf, &blitInfo);
    }

    SDL_SubmitGPUCommandBuffer(cmdBuf);
    return true;
}

void Two_Body_Simulation::Quit()
{
    Particles.quit();
}

bool Two_Body_Simulation::DrawUI()
{
    if (ImGui::Begin("Two_Body_Simulation Controller###ProjectUI"))
    {
        ImGui::SeparatorText("Options");
        // ImGui::SliderFloat("Radius", &Radius, 0.0f, 200.0f);
    }
    ImGui::End();

    return true;
}
