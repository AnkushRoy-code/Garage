#include "Core/Common/pch.h"

#include "Core/Common/Common.h"
#include "Core/Context.h"

#include "Projects/N_Body_Simulation/src/Main.h"
#include "Core/EventHandler.h"
#include "Projects/N_Body_Simulation/src/Particle.h"

void N_Body_Simulation::InitialiseTransferBuffersAndParticleContainer()
{
    const SDL_GPUTransferBufferCreateInfo tBufCreateInfo {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size  = (Uint32)(ParticleContainer::count * sizeof(ParticleDataSend)),
    };

    TransferBuffer = SDL_CreateGPUTransferBuffer(g_Context.RenderData.Device, &tBufCreateInfo);

    const SDL_GPUBufferCreateInfo newBufCreateInfo {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size  = (Uint32)(ParticleContainer::count * sizeof(ParticleDataSend)),

    };

    DataBuffer = SDL_CreateGPUBuffer(g_Context.RenderData.Device, &newBufCreateInfo);
}

bool N_Body_Simulation::Init()
{
    hasUI = true;

    // pipeline creation
    SDL_GPUShader *vertShader =
        Common::LoadShader(g_Context.RenderData.Device, "DrawCircle.vert", 0, 1, 1, 0);
    SDL_GPUShader *fragShader =
        Common::LoadShader(g_Context.RenderData.Device, "DrawCircle.frag", 0, 0, 0, 0);

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
        .format      = SDL_GetGPUSwapchainTextureFormat(g_Context.RenderData.Device,
                                                        g_Context.RenderData.Window),
        .blend_state = blendState,
    };

    const SDL_GPUGraphicsPipelineTargetInfo targetInfo {
        .color_target_descriptions = &colorDesc,
        .num_color_targets         = 1,
    };

    const SDL_GPUMultisampleState mState = {
        .sample_count = g_Context.RenderData.SampleCount,  // MSAA or not
    };

    const SDL_GPUGraphicsPipelineCreateInfo createInfo {
        .vertex_shader     = vertShader,
        .fragment_shader   = fragShader,
        .primitive_type    = SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP,
        .multisample_state = mState,
        .target_info       = targetInfo,
    };

    RenderPipeline = SDL_CreateGPUGraphicsPipeline(g_Context.RenderData.Device, &createInfo);

    SDL_ReleaseGPUShader(g_Context.RenderData.Device, vertShader);
    SDL_ReleaseGPUShader(g_Context.RenderData.Device, fragShader);

    InitialiseTransferBuffersAndParticleContainer();

    Particles.Init();
    return true;
}

bool N_Body_Simulation::Update()
{
    Particles.Update();
    return true;
}

bool N_Body_Simulation::Draw()
{
    const float w = g_Context.AppState.ProjectWindowX;
    const float h = g_Context.AppState.ProjectWindowY;

    const glm::mat4x4 projection = glm::ortho(0.0f, w * 2, h * 2, 0.0f, 0.0f, -1.0f);
    // const glm::mat4x4 finalProjection = projection;
    const glm::mat4x4 offset = glm::translate(glm::mat4(1.0f), glm::vec3(w / 2, h / 2, 0.0f));
    const glm::mat4x4 finalProjection = projection * offset;

    SDL_GPUCommandBuffer *cmdBuf = SDL_AcquireGPUCommandBuffer(g_Context.RenderData.Device);
    if (cmdBuf == nullptr) { std::cerr << "unable to aquire command buffer\n"; }

    if (g_Context.RenderData.ProjectTexture)
    {
        // upload the data to transfer buffer
        auto *dataPtr = static_cast<ParticleDataSend *>(
            SDL_MapGPUTransferBuffer(g_Context.RenderData.Device, TransferBuffer, true));

        for (int i = 0; i < ParticleContainer::count; i++)
        {
            dataPtr[i].Color    = Particles.ParticleVec[i].Color;
            dataPtr[i].Position = Particles.ParticleVec[i].Position;
            dataPtr[i].Radius   = Particles.ParticleVec[i].Radius;
            dataPtr[i]._padding = 0.0f;
        }

        SDL_UnmapGPUTransferBuffer(g_Context.RenderData.Device, TransferBuffer);

        // upload the data to gpu from transfer buffers
        const SDL_GPUTransferBufferLocation transferBufferLocation {
            .transfer_buffer = TransferBuffer,
            .offset          = 0,
        };

        const SDL_GPUBufferRegion bufferRegion {
            .buffer = DataBuffer,
            .offset = 0,
            .size   = (Uint32)(ParticleContainer::count * sizeof(ParticleDataSend)),
        };

        SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuf);
        SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &bufferRegion, true);
        SDL_EndGPUCopyPass(copyPass);

        SDL_GPUColorTargetInfo tinfo {.texture     = g_Context.RenderData.ProjectTexture,
                                      .clear_color = {0.094f, 0.094f, 0.145f, 1.00f},
                                      .load_op     = SDL_GPU_LOADOP_CLEAR,
                                      .store_op    = SDL_GPU_STOREOP_STORE,
                                      .cycle       = false};

        if (g_Context.RenderData.SampleCount == SDL_GPU_SAMPLECOUNT_1)
        {
            tinfo.store_op = SDL_GPU_STOREOP_STORE;
        }
        else
        {
            tinfo.store_op        = SDL_GPU_STOREOP_RESOLVE;
            tinfo.resolve_texture = g_Context.RenderData.ResolveTexture;
        }

        g_Context.RenderData.ProjectPass = SDL_BeginGPURenderPass(cmdBuf, &tinfo, 1, nullptr);

        SDL_BindGPUGraphicsPipeline(g_Context.RenderData.ProjectPass, RenderPipeline);
        SDL_BindGPUVertexStorageBuffers(g_Context.RenderData.ProjectPass, 0, &DataBuffer, 1);

        SDL_PushGPUVertexUniformData(cmdBuf, 0, &finalProjection, sizeof(glm::mat4x4));

        SDL_DrawGPUPrimitives(g_Context.RenderData.ProjectPass, 4, ParticleContainer::count, 0, 0);

        SDL_EndGPURenderPass(g_Context.RenderData.ProjectPass);

        SDL_GPUTexture *blitSourceTexture =
            (tinfo.resolve_texture != nullptr) ? tinfo.resolve_texture : tinfo.texture;

        const SDL_GPUBlitRegion blitSrc = {
            .texture = blitSourceTexture,
            .w       = (Uint32)g_Context.AppState.ProjectWindowX,
            .h       = (Uint32)g_Context.AppState.ProjectWindowY,
        };

        SDL_BindGPUFragmentStorageTextures(g_Context.RenderData.ProjectPass, 1,
                                           &g_Context.RenderData.ProjectTexture, 2);

        const SDL_GPUBlitRegion blitDst = {
            .texture = g_Context.RenderData.ProjectTexture,
            .w       = (Uint32)g_Context.AppState.ProjectWindowX,
            .h       = (Uint32)g_Context.AppState.ProjectWindowY,
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

void N_Body_Simulation::Quit()
{
    Particles.Quit();
}

bool N_Body_Simulation::DrawUI()
{
    if (ImGui::Begin("N_Body_Simulation Controller###ProjectUI"))
    {
        ImGui::SeparatorText("Options");

        static const std::string names[4] = {"Two Bodies", "Four Bodies", "Circular arrangement",
                                             "Grid Arrangement"};

        static unsigned int index = 0;

        if (ImGui::BeginCombo("Simulation", names[index].c_str()))
        {
            for (int i = 0; i < Particles.ParticlesData.size(); i++)
            {
                const bool isSelected = (index == i);
                if (ImGui::Selectable(names[i].c_str(), isSelected))
                {
                    if (i != index)
                    {
                        index = i;
                        Particles.InitData(index);
                        InitialiseTransferBuffersAndParticleContainer();
                    }
                }
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Restart Simulation")) { Particles.InitData(index); }

        using CK    = Core::KEY;
        bool right  = g_Context.EventHandler.GetEventHeld(CK::MOUSE_RIGHT_CLICK);
        bool left   = g_Context.EventHandler.GetEventHeld(CK::MOUSE_LEFT_CLICK);
        bool middle = g_Context.EventHandler.GetEventHeld(CK::MOUSE_MIDDLE_CLICK);
        bool scroll = g_Context.EventHandler.GetEventHeld(CK::MOUSE_ROLL);

        static glm::vec2 scrollValue = {0.0f, 0.0f};

        if (scroll)
        {
            scrollValue.x += g_Context.AppState.HorizontalScroll;
            scrollValue.y += g_Context.AppState.VerticalScroll;
        }

        ImGui::Text("Right Click: %s", (right ? "True" : "False"));
        ImGui::Text("Left Click: %s", (left ? "True" : "False"));
        ImGui::Text("Middle Click: %s", (middle ? "True" : "False"));
        ImGui::Text("Scroll Click: %s", (scroll ? "True" : "False"));
        ImGui::Text("Scroll.x: %.3f", scrollValue.x);
        ImGui::Text("Scroll.y: %.3f", scrollValue.y);
    }
    ImGui::End();

    return true;
}
