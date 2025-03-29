#include "Main.h"
#include "Common/Common.h"
#include "Core/Context.h"
#include "imgui.h"
#include <SDL3/SDL_gpu.h>
#include <cmath>
#include <iostream>

static const int SPRITE_COUNT = 100;

data BoidsData {};

bool Boids::Init()
{
    hasUI = true;

    SDL_GPUShader *vertShader =
        Common::LoadShader(gContext.renderData.device, "PullSpriteBatch.vert", 0, 1, 1, 0);
    SDL_GPUShader *fragShader =
        Common::LoadShader(gContext.renderData.device, "TexturedQuadColor.frag", 0, 0, 0, 0);

    SDL_srand(0);

    SDL_GPUColorTargetBlendState blendState {};
    blendState.enable_blend          = true;
    blendState.color_blend_op        = SDL_GPU_BLENDOP_ADD;
    blendState.alpha_blend_op        = SDL_GPU_BLENDOP_ADD;
    blendState.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    blendState.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    blendState.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    blendState.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    SDL_GPUColorTargetDescription colorDesc {};
    colorDesc.format =
        SDL_GetGPUSwapchainTextureFormat(gContext.renderData.device, gContext.renderData.window);
    colorDesc.blend_state = blendState;

    SDL_GPUGraphicsPipelineTargetInfo targetInfo {};
    targetInfo.num_color_targets         = 1;
    targetInfo.color_target_descriptions = &colorDesc;

    SDL_GPUGraphicsPipelineCreateInfo createInfo {};
    createInfo.target_info     = targetInfo;
    createInfo.primitive_type  = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    createInfo.vertex_shader   = vertShader;
    createInfo.fragment_shader = fragShader;

    renderPipeline = SDL_CreateGPUGraphicsPipeline(gContext.renderData.device, &createInfo);

    SDL_ReleaseGPUShader(gContext.renderData.device, vertShader);
    SDL_ReleaseGPUShader(gContext.renderData.device, fragShader);

    SDL_GPUTransferBufferCreateInfo tBufCreateInfo {};
    tBufCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    tBufCreateInfo.size  = SPRITE_COUNT * sizeof(SpriteInstance);

    SpriteDataTransferBuffer =
        SDL_CreateGPUTransferBuffer(gContext.renderData.device, &tBufCreateInfo);

    SDL_GPUBufferCreateInfo newBufCreateInfo {};
    newBufCreateInfo.usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ;
    newBufCreateInfo.size  = SPRITE_COUNT * sizeof(SpriteInstance);

    SpriteDataBuffer = SDL_CreateGPUBuffer(gContext.renderData.device, &newBufCreateInfo);

    BoidsData.r        = 0.0f;
    BoidsData.g        = 1.0f;
    BoidsData.b        = 1.0f;
    BoidsData.a        = 1.0f;
    BoidsData.rotation = 1;
    return true;
}
bool Boids::Update()
{
    PROFILE_SCOPE_N("Boids Update");
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

    float w = gContext.appState.ProjectWindowX;
    float h = gContext.appState.ProjectWindowY;

    Matrix4x4 cameraMatrix = Matrix4x4_CreateOrthographicOffCenter(0, w, h, 0, 0, -1);

    SDL_GPUCommandBuffer *cmdBuf = SDL_AcquireGPUCommandBuffer(gContext.renderData.device);
    if (cmdBuf == nullptr) { std::cerr << "unable to aquire command buffer\n"; }

    if (gContext.renderData.projectTexture)
    {
        auto *dataPtr = static_cast<SpriteInstance *>(
            SDL_MapGPUTransferBuffer(gContext.renderData.device, SpriteDataTransferBuffer, true));

        for (Uint32 i = 0; i < SPRITE_COUNT; i++)
        {
            dataPtr[i].x        = i * 50 % (int)w;
            dataPtr[i].y        = i * 50 % (int)h;
            dataPtr[i].z        = 0.0f;
            dataPtr[i].r        = BoidsData.r;
            dataPtr[i].g        = BoidsData.g;
            dataPtr[i].b        = BoidsData.b;
            dataPtr[i].a        = BoidsData.a;
            dataPtr[i].rotation = SDL_PI_F * BoidsData.rotation;
        }

        SDL_UnmapGPUTransferBuffer(gContext.renderData.device, SpriteDataTransferBuffer);

        SDL_GPUTransferBufferLocation transferBufferLocation {};
        transferBufferLocation.transfer_buffer = SpriteDataTransferBuffer;
        transferBufferLocation.offset          = 0;

        SDL_GPUBufferRegion bufferRegion {};
        bufferRegion.buffer = SpriteDataBuffer;
        bufferRegion.offset = 0;
        bufferRegion.size   = SPRITE_COUNT * sizeof(SpriteInstance);

        SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuf);
        SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &bufferRegion, true);
        SDL_EndGPUCopyPass(copyPass);

        SDL_GPUColorTargetInfo tinfo {};
        tinfo.texture     = gContext.renderData.projectTexture;
        tinfo.cycle       = false;
        tinfo.load_op     = SDL_GPU_LOADOP_CLEAR;
        tinfo.store_op    = SDL_GPU_STOREOP_STORE;
        tinfo.clear_color = {0.45f, 0.55f, 0.60f, 1.00f},

        gContext.renderData.projectPass = SDL_BeginGPURenderPass(cmdBuf, &tinfo, 1, nullptr);

        SDL_BindGPUGraphicsPipeline(gContext.renderData.projectPass, renderPipeline);
        SDL_BindGPUVertexStorageBuffers(gContext.renderData.projectPass, 0, &SpriteDataBuffer, 1);

        SDL_PushGPUVertexUniformData(cmdBuf, 0, &cameraMatrix, sizeof(Matrix4x4));

        SDL_DrawGPUPrimitives(gContext.renderData.projectPass, SPRITE_COUNT * 3, 1, 0, 1);

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
        ImGui::SliderFloat("R", &BoidsData.r, 0, 1);
        ImGui::SliderFloat("G", &BoidsData.g, 0, 1);
        ImGui::SliderFloat("B", &BoidsData.b, 0, 1);
        ImGui::SliderFloat("A", &BoidsData.a, 0, 1);
        ImGui::SliderFloat("Rotation", &BoidsData.rotation, -1, 1);

        ImGui::End();
    }

    return true;
}
