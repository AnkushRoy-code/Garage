#include "Projects/Boids/src/Main.h"

#include "Core/Common/Common.h"
#include "Core/Context.h"
#include "Utils/Time.h"
#include "imgui.h"

bool Boids::Init()
{
    hasUI = true;

    // pipeline creation
    SDL_GPUShader *vertShader =
        Common::LoadShader(g_Context.RenderData.Device, "Boids.vert", 0, 1, 1, 0);
    SDL_GPUShader *fragShader =
        Common::LoadShader(g_Context.RenderData.Device, "Boids.frag", 0, 0, 0, 0);

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
        .primitive_type    = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .multisample_state = mState,
        .target_info       = targetInfo,
    };

    m_RenderPipeline = SDL_CreateGPUGraphicsPipeline(g_Context.RenderData.Device, &createInfo);

    SDL_ReleaseGPUShader(g_Context.RenderData.Device, vertShader);
    SDL_ReleaseGPUShader(g_Context.RenderData.Device, fragShader);

    // Transfer buffers creation
    const SDL_GPUTransferBufferCreateInfo tBufCreateInfo {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size  = (Uint32)(m_BoidsContainer.NumBoids() * sizeof(BoidsDataSend)),
    };

    m_BoidsDataTransferBuffer =
        SDL_CreateGPUTransferBuffer(g_Context.RenderData.Device, &tBufCreateInfo);

    const SDL_GPUBufferCreateInfo newBufCreateInfo {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size  = (Uint32)(m_BoidsContainer.NumBoids() * sizeof(BoidsDataSend)),

    };

    m_BoidsDataBuffer = SDL_CreateGPUBuffer(g_Context.RenderData.Device, &newBufCreateInfo);

    // rgb pipeline creation

    SDL_GPUShader *rgbVertShader =
        Common::LoadShader(g_Context.RenderData.Device, "BoidsRGB.vert", 0, 1, 1, 0);
    SDL_GPUShader *rgbFragShader =
        Common::LoadShader(g_Context.RenderData.Device, "BoidsRGB.frag", 0, 0, 0, 0);
    ;

    const SDL_GPUColorTargetBlendState rgbBlendState = blendState;

    const SDL_GPUColorTargetDescription rgbColorDesc {
        .format      = SDL_GetGPUSwapchainTextureFormat(g_Context.RenderData.Device,
                                                        g_Context.RenderData.Window),
        .blend_state = rgbBlendState,
    };

    const SDL_GPUGraphicsPipelineTargetInfo rgbTargetInfo {
        .color_target_descriptions = &rgbColorDesc,
        .num_color_targets         = 1,
    };

    const SDL_GPUMultisampleState rgbState = {
        .sample_count = g_Context.RenderData.SampleCount,  // MSAA or not
    };

    const SDL_GPUGraphicsPipelineCreateInfo rgbCreateInfo {
        .vertex_shader     = rgbVertShader,
        .fragment_shader   = rgbFragShader,
        .primitive_type    = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .multisample_state = rgbState,
        .target_info       = rgbTargetInfo,
    };

    m_RGB_RenderPipeline = SDL_CreateGPUGraphicsPipeline(g_Context.RenderData.Device, &createInfo);

    SDL_ReleaseGPUShader(g_Context.RenderData.Device, rgbVertShader);
    SDL_ReleaseGPUShader(g_Context.RenderData.Device, rgbFragShader);

    // Transfer buffers creation
    const SDL_GPUTransferBufferCreateInfo rgbTBufCreateInfo {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size  = (Uint32)(m_BoidsContainer.NumBoids() * sizeof(RGB_BoidDataSend)),
    };

    m_RGB_BoidsDataTransferBuffer =
        SDL_CreateGPUTransferBuffer(g_Context.RenderData.Device, &rgbTBufCreateInfo);

    const SDL_GPUBufferCreateInfo rgbNewBufCreateInfo {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size  = (Uint32)(m_BoidsContainer.NumBoids() * sizeof(RGB_BoidDataSend)),

    };

    m_RGB_BoidsDataBuffer = SDL_CreateGPUBuffer(g_Context.RenderData.Device, &rgbNewBufCreateInfo);
    m_BoidsContainer.Init();
    return true;
}

bool Boids::Update()
{
    PROFILE_SCOPE_N("Boids Update");
    m_BoidsContainer.Update();
    return true;
}

bool Boids::Draw()
{
    PROFILE_SCOPE_N("Boids Draw");

    const float w = g_Context.AppState.ProjectWindowX;
    const float h = g_Context.AppState.ProjectWindowY;

    const glm::mat4x4 cameraMatrix = glm::ortho(0.0f, w, h, 0.0f, 0.0f, -1.0f);

    SDL_GPUCommandBuffer *cmdBuf = SDL_AcquireGPUCommandBuffer(g_Context.RenderData.Device);
    if (cmdBuf == nullptr) { std::cerr << "unable to aquire command buffer\n"; }

    if (g_Context.RenderData.ProjectTexture)
    {
        // upload the data to transfer buffer
        if (!s_rgb)
        {
            auto *dataPtr = static_cast<BoidsDataSend *>(SDL_MapGPUTransferBuffer(
                g_Context.RenderData.Device, m_BoidsDataTransferBuffer, true));

            static int i = 0;
            for (const auto &boid: m_BoidsContainer.GetBoids())
            {
                dataPtr[i].x        = boid.Position.x;
                dataPtr[i].y        = boid.Position.y;
                dataPtr[i].z        = 0.0f;  // why do I even have this?
                dataPtr[i].r        = m_BoidsContainer.Color.r;
                dataPtr[i].g        = m_BoidsContainer.Color.g;
                dataPtr[i].b        = m_BoidsContainer.Color.b;
                dataPtr[i].a        = m_BoidsContainer.Color.a;
                dataPtr[i].Rotation = boid.Rotation - SDL_PI_F * 0.5;
                i++;
            }
            i = 0;
            SDL_UnmapGPUTransferBuffer(g_Context.RenderData.Device, m_BoidsDataTransferBuffer);

            // upload the data to gpu from transfer buffers
            const SDL_GPUTransferBufferLocation transferBufferLocation {
                .transfer_buffer = m_BoidsDataTransferBuffer,
                .offset          = 0,
            };

            const SDL_GPUBufferRegion bufferRegion {
                .buffer = m_BoidsDataBuffer,
                .offset = 0,
                .size   = (Uint32)(m_BoidsContainer.NumBoids() * sizeof(BoidsDataSend)),
            };

            SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuf);
            SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &bufferRegion, true);
            SDL_EndGPUCopyPass(copyPass);
        }  // if (!rgb)
        else
        {
            auto *dataPtr = static_cast<RGB_BoidDataSend *>(SDL_MapGPUTransferBuffer(
                g_Context.RenderData.Device, m_RGB_BoidsDataTransferBuffer, true));

            static int i = 0;
            for (const auto &boid: m_BoidsContainer.GetBoids())
            {
                dataPtr[i].x        = boid.Position.x;
                dataPtr[i].y        = boid.Position.y;
                dataPtr[i].Time     = Utils::Time::GetTicks();
                dataPtr[i].Rotation = boid.Rotation - SDL_PI_F * 0.5;
                i++;
            }
            i = 0;
            SDL_UnmapGPUTransferBuffer(g_Context.RenderData.Device, m_RGB_BoidsDataTransferBuffer);

            // upload the data to gpu from transfer buffers
            const SDL_GPUTransferBufferLocation transferBufferLocation {
                .transfer_buffer = m_RGB_BoidsDataTransferBuffer,
                .offset          = 0,
            };

            const SDL_GPUBufferRegion bufferRegion {
                .buffer = m_RGB_BoidsDataBuffer,
                .offset = 0,
                .size   = (Uint32)(m_BoidsContainer.NumBoids() * sizeof(RGB_BoidDataSend)),
            };

            SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuf);
            SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &bufferRegion, true);
            SDL_EndGPUCopyPass(copyPass);
        }

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

        if (!s_rgb)
        {
            SDL_BindGPUGraphicsPipeline(g_Context.RenderData.ProjectPass, m_RenderPipeline);
            SDL_BindGPUVertexStorageBuffers(g_Context.RenderData.ProjectPass, 0, &m_BoidsDataBuffer,
                                            1);

            SDL_PushGPUVertexUniformData(cmdBuf, 0, &cameraMatrix, sizeof(glm::mat4x4));

            SDL_DrawGPUPrimitives(g_Context.RenderData.ProjectPass, m_BoidsContainer.NumBoids() * 3,
                                  1, 0, 1);

            SDL_EndGPURenderPass(g_Context.RenderData.ProjectPass);
        }
        else
        {
            SDL_BindGPUGraphicsPipeline(g_Context.RenderData.ProjectPass, m_RGB_RenderPipeline);
            SDL_BindGPUVertexStorageBuffers(g_Context.RenderData.ProjectPass, 0,
                                            &m_RGB_BoidsDataBuffer, 1);

            SDL_PushGPUVertexUniformData(cmdBuf, 0, &cameraMatrix, sizeof(glm::mat4x4));

            auto res = glm::vec2 {w, h};
            SDL_PushGPUFragmentUniformData(cmdBuf, 0, &res, sizeof(glm::vec2));

            SDL_DrawGPUPrimitives(g_Context.RenderData.ProjectPass, m_BoidsContainer.NumBoids() * 3,
                                  1, 0, 1);

            SDL_EndGPURenderPass(g_Context.RenderData.ProjectPass);
        }

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

void Boids::Quit()
{
    m_BoidsContainer.Quit();
    SDL_ReleaseGPUGraphicsPipeline(g_Context.RenderData.Device, m_RenderPipeline);
    SDL_ReleaseGPUTransferBuffer(g_Context.RenderData.Device, m_BoidsDataTransferBuffer);
    SDL_ReleaseGPUBuffer(g_Context.RenderData.Device, m_BoidsDataBuffer);

    SDL_ReleaseGPUGraphicsPipeline(g_Context.RenderData.Device, m_RGB_RenderPipeline);
    SDL_ReleaseGPUTransferBuffer(g_Context.RenderData.Device, m_RGB_BoidsDataTransferBuffer);
    SDL_ReleaseGPUBuffer(g_Context.RenderData.Device, m_RGB_BoidsDataBuffer);
}

bool Boids::DrawUI()
{
    if (ImGui::Begin("Boids Controller###ProjectUI"))
    {
        ImGui::SeparatorText("Options");
        ImGui::Text("Seperation");
        ImGui::SliderFloat("###tabulabul", &m_BoidsContainer.Seperation, 0.0f, 0.1f);
        ImGui::Text("Alignment");
        ImGui::SliderFloat("###loremipusummyass", &m_BoidsContainer.Alignment, 0.0f, 0.1f);
        static float a = 0.05;
        ImGui::Text("Cohesion");
        if (ImGui::SliderFloat("###someRandomfuckingIdthatisnotempty", &a, 0.0f, 0.1f))
        {  // imgui is not good with small values
            m_BoidsContainer.Cohesion = 0.001 * a;
        }
        static bool dontuse = false;
        ImGui::Checkbox("RGB Lighting", &dontuse);
        // ImGui::Checkbox("RGB Lighting", &rgb);

        // Helper
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted("This is under construction");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        if (!s_rgb)
        {
            ImGui::Text("Boids Color Picker");
            ImGui::ColorEdit4("###abultabul", (float *)&m_BoidsContainer.Color,
                              ImGuiColorEditFlags_AlphaPreviewHalf);
        }
    }
    ImGui::End();
    return true;
}
