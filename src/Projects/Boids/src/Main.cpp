#include "Main.h"
#include "Common/Common.h"
#include "Core/Context.h"
#include "SDL3/SDL_stdinc.h"
#include "imgui.h"
#include <SDL3/SDL_gpu.h>
#include <cmath>
#include <iostream>

bool Boids::Init()
{
    hasUI = true;

    SDL_GPUShader *vertShader =
        Common::LoadShader(gContext.renderData.device, "PullSpriteBatch.vert", 0, 1, 1, 0);
    SDL_GPUShader *fragShader =
        Common::LoadShader(gContext.renderData.device, "TexturedQuadColor.frag", 0, 0, 0, 0);

    SDL_srand(0);

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

    const SDL_GPUGraphicsPipelineCreateInfo createInfo {
        .vertex_shader   = vertShader,
        .fragment_shader = fragShader,
        .primitive_type  = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .target_info     = targetInfo,
    };

    renderPipeline = SDL_CreateGPUGraphicsPipeline(gContext.renderData.device, &createInfo);

    SDL_ReleaseGPUShader(gContext.renderData.device, vertShader);
    SDL_ReleaseGPUShader(gContext.renderData.device, fragShader);

    const SDL_GPUTransferBufferCreateInfo tBufCreateInfo {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size  = static_cast<Uint32>(boidsContainer.numBoids() * sizeof(SpriteInstance)),
    };

    SpriteDataTransferBuffer =
        SDL_CreateGPUTransferBuffer(gContext.renderData.device, &tBufCreateInfo);

    const SDL_GPUBufferCreateInfo newBufCreateInfo {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size  = static_cast<Uint32>(boidsContainer.numBoids() * sizeof(SpriteInstance)),

    };

    SpriteDataBuffer = SDL_CreateGPUBuffer(gContext.renderData.device, &newBufCreateInfo);

    boidsContainer.init();
    return true;
}
bool Boids::Update()
{
    PROFILE_SCOPE_N("Boids Update");
    boidsContainer.update();
    return true;
}

Matrix4x4 Matrix4x4_CreateOrthographicOffCenter(
    float left, float right, float bottom, float top, float zNearPlane, float zFarPlane)
{
    return {2.0f / (right - left),
            0,
            0,
            0,
            0,
            2.0f / (top - bottom),
            0,
            0,
            0,
            0,
            1.0f / (zNearPlane - zFarPlane),
            0,
            (left + right) / (left - right),
            (top + bottom) / (bottom - top),
            zNearPlane / (zNearPlane - zFarPlane),
            1};
}

bool Boids::Draw()
{
    PROFILE_SCOPE_N("Boids Draw");

    const float w = gContext.appState.ProjectWindowX;
    const float h = gContext.appState.ProjectWindowY;

    const Matrix4x4 cameraMatrix = Matrix4x4_CreateOrthographicOffCenter(0, w, h, 0, 0, -1);

    SDL_GPUCommandBuffer *cmdBuf = SDL_AcquireGPUCommandBuffer(gContext.renderData.device);
    if (cmdBuf == nullptr) { std::cerr << "unable to aquire command buffer\n"; }

    if (gContext.renderData.projectTexture)
    {
        auto *dataPtr = static_cast<SpriteInstance *>(
            SDL_MapGPUTransferBuffer(gContext.renderData.device, SpriteDataTransferBuffer, true));

        static int i = 0;

        for (const auto &boid: boidsContainer.getBoids())
        {
            dataPtr[i].x        = boid.Position.x;
            dataPtr[i].y        = boid.Position.y;
            dataPtr[i].z        = 0.0f;  // why do I even have this?
            dataPtr[i].r        = boidsContainer.Color.r;
            dataPtr[i].g        = boidsContainer.Color.g;
            dataPtr[i].b        = boidsContainer.Color.b;
            dataPtr[i].a        = boidsContainer.Color.a;
            dataPtr[i].rotation = boid.Rotation - SDL_PI_F * 0.5;
            i++;
        }
        i = 0;

        SDL_UnmapGPUTransferBuffer(gContext.renderData.device, SpriteDataTransferBuffer);

        const SDL_GPUTransferBufferLocation transferBufferLocation {
            .transfer_buffer = SpriteDataTransferBuffer,
            .offset          = 0,
        };

        const SDL_GPUBufferRegion bufferRegion {
            .buffer = SpriteDataBuffer,
            .offset = 0,
            .size   = static_cast<Uint32>(boidsContainer.numBoids() * sizeof(SpriteInstance)),
        };

        SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuf);
        SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &bufferRegion, true);
        SDL_EndGPUCopyPass(copyPass);

        const SDL_GPUColorTargetInfo tinfo {.texture     = gContext.renderData.projectTexture,
                                            .clear_color = {0.094f, 0.094f, 0.145f, 1.00f},
                                            .load_op     = SDL_GPU_LOADOP_CLEAR,
                                            .store_op    = SDL_GPU_STOREOP_STORE,
                                            .cycle       = false};

        gContext.renderData.projectPass = SDL_BeginGPURenderPass(cmdBuf, &tinfo, 1, nullptr);

        SDL_BindGPUGraphicsPipeline(gContext.renderData.projectPass, renderPipeline);
        SDL_BindGPUVertexStorageBuffers(gContext.renderData.projectPass, 0, &SpriteDataBuffer, 1);

        SDL_PushGPUVertexUniformData(cmdBuf, 0, &cameraMatrix, sizeof(Matrix4x4));

        SDL_DrawGPUPrimitives(gContext.renderData.projectPass, boidsContainer.numBoids() * 3, 1, 0,
                              1);

        SDL_EndGPURenderPass(gContext.renderData.projectPass);
    }

    SDL_SubmitGPUCommandBuffer(cmdBuf);
    return true;
}

void Boids::Quit()
{
    SDL_ReleaseGPUGraphicsPipeline(gContext.renderData.device, renderPipeline);
    SDL_ReleaseGPUTransferBuffer(gContext.renderData.device, SpriteDataTransferBuffer);
    SDL_ReleaseGPUBuffer(gContext.renderData.device, SpriteDataBuffer);
}

bool Boids::DrawUI()
{
    if (ImGui::Begin("Boids Controller###ProjectUI"))
    {
        ImGui::SeparatorText("Options");
        ImGui::TextWrapped("Boids Color Picker");
        ImGui::ColorEdit4("", (float *)&boidsContainer.Color, ImGuiColorEditFlags_AlphaPreviewHalf);
        ImGui::SliderFloat("Seperation", &boidsContainer.seperation, 0.0f, 0.1f);
        ImGui::SliderFloat("Alignment", &boidsContainer.alignment, 0.0f, 0.1f);
        static float a = 0.05;
        if (ImGui::SliderFloat("Cohesion", &a, 0.0f, 0.1f))
        {  // imgui is not good with small values
            boidsContainer.cohesion = 0.001 * a;
        }

        ImGui::End();
    }

    return true;
}
