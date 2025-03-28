#include "Main.h"
#include "Common/Common.h"
#include "Common/SDL_Exception.h"
#include "Core/Context.h"
#include "SDL3/SDL_gpu.h"
#include "Utils/Time.h"
#include <cmath>
#include <iostream>

static const int SPRITE_COUNT = 400;

bool Boids::Init()
{
    hasUI = true;

    SDL_GPUShader *vertShader =
        Common::LoadShader(gContext.renderData.device, "PullSpriteBatch.vert", 0, 1, 1, 0);
    SDL_GPUShader *fragShader =
        Common::LoadShader(gContext.renderData.device, "TexturedQuadColor.frag", 1, 0, 0, 0);

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

    RenderPipeline = SDL_CreateGPUGraphicsPipeline(gContext.renderData.device, &createInfo);

    SDL_ReleaseGPUShader(gContext.renderData.device, vertShader);
    SDL_ReleaseGPUShader(gContext.renderData.device, fragShader);

    SDL_Surface *imageData = Common::LoadImage("Boid", 4);
    if (imageData == nullptr) { throw SDL_Exception("Couldn't load ImageData!"); }

    SDL_GPUTransferBufferCreateInfo bufCreateInfo {};
    bufCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    bufCreateInfo.size  = imageData->w * imageData->h * 4;

    SDL_GPUTransferBuffer *textureTransferBuffer =
        SDL_CreateGPUTransferBuffer(gContext.renderData.device, &bufCreateInfo);

    auto textureTransferPtr =
        SDL_MapGPUTransferBuffer(gContext.renderData.device, textureTransferBuffer, false);
    SDL_memcpy(textureTransferPtr, imageData->pixels, imageData->w * imageData->h * 4);
    SDL_UnmapGPUTransferBuffer(gContext.renderData.device, textureTransferBuffer);

    SDL_GPUTextureCreateInfo texCreateInfo {};
    texCreateInfo.type                 = SDL_GPU_TEXTURETYPE_2D;
    texCreateInfo.format               = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    texCreateInfo.width                = imageData->w;
    texCreateInfo.height               = imageData->h;
    texCreateInfo.layer_count_or_depth = 1;
    texCreateInfo.num_levels           = 1;
    texCreateInfo.usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER;

    Texture = SDL_CreateGPUTexture(gContext.renderData.device, &texCreateInfo);

    SDL_GPUSamplerCreateInfo samplerCreateInfo {};
    samplerCreateInfo.min_filter     = SDL_GPU_FILTER_NEAREST;
    samplerCreateInfo.mag_filter     = SDL_GPU_FILTER_NEAREST;
    samplerCreateInfo.mipmap_mode    = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
    samplerCreateInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerCreateInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerCreateInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

    Sampler = SDL_CreateGPUSampler(gContext.renderData.device, &samplerCreateInfo);

    SDL_GPUTransferBufferCreateInfo tBufCreateInfo {};
    tBufCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    tBufCreateInfo.size  = SPRITE_COUNT * sizeof(SpriteInstance);

    SpriteDataTransferBuffer =
        SDL_CreateGPUTransferBuffer(gContext.renderData.device, &tBufCreateInfo);

    SDL_GPUBufferCreateInfo newBufCreateInfo {};
    newBufCreateInfo.usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ;
    newBufCreateInfo.size  = SPRITE_COUNT * sizeof(SpriteInstance);

    SpriteDataBuffer = SDL_CreateGPUBuffer(gContext.renderData.device, &newBufCreateInfo);

    // Transfer the up-front data
    SDL_GPUCommandBuffer *uploadCmdBuf = SDL_AcquireGPUCommandBuffer(gContext.renderData.device);
    SDL_GPUCopyPass *copyPass          = SDL_BeginGPUCopyPass(uploadCmdBuf);

    SDL_GPUTextureTransferInfo tTransferInfo {};
    tTransferInfo.transfer_buffer = textureTransferBuffer;
    tTransferInfo.offset          = 0;

    SDL_GPUTextureRegion region {};
    region.texture = Texture;
    region.w       = imageData->w;
    region.h       = imageData->h;
    region.d       = 1;

    SDL_UploadToGPUTexture(copyPass, &tTransferInfo, &region, false);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(uploadCmdBuf);

    SDL_DestroySurface(imageData);
    SDL_ReleaseGPUTransferBuffer(gContext.renderData.device, textureTransferBuffer);

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

        static float uCoords[4] = {0.0f, 0.5f, 0.0f, 0.5f};
        static float vCoords[4] = {0.0f, 0.0f, 0.5f, 0.5f};

        for (Uint32 i = 0; i < SPRITE_COUNT; i += 1)
        {
            Sint32 ravioli = 0;
            dataPtr[i].x   = (i * 200) % (int)w;
            dataPtr[i].y   = (i * 100) % (int)h;
            dataPtr[i].z   = 0;
            // dataPtr[i].rotation = SDL_PI_F * 2 * std::sin(Utils::Time::getTicks() / 1000.0f);
            dataPtr[i].rotation = SDL_PI_F * 2;
            dataPtr[i].w        = 32;
            dataPtr[i].h        = 32;
            dataPtr[i].tex_u    = uCoords[ravioli];
            dataPtr[i].tex_v    = vCoords[ravioli];
            dataPtr[i].tex_w    = 1.0f;
            dataPtr[i].tex_h    = 1.0f;
            dataPtr[i].r        = 1.0f;
            dataPtr[i].g        = 1.0f;
            dataPtr[i].b        = 1.0f;
            dataPtr[i].a        = 1.0f;
        }

        SDL_UnmapGPUTransferBuffer(gContext.renderData.device, SpriteDataTransferBuffer);

        SDL_GPUTransferBufferLocation sm {};
        sm.transfer_buffer = SpriteDataTransferBuffer;
        sm.offset          = 0;

        SDL_GPUBufferRegion sr {};
        sr.buffer = SpriteDataBuffer;
        sr.offset = 0;
        sr.size   = SPRITE_COUNT * sizeof(SpriteInstance);

        SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuf);
        SDL_UploadToGPUBuffer(copyPass, &sm, &sr, true);
        SDL_EndGPUCopyPass(copyPass);

        SDL_GPUColorTargetInfo tinfo {};
        tinfo.texture     = gContext.renderData.projectTexture;
        tinfo.cycle       = false;
        tinfo.load_op     = SDL_GPU_LOADOP_CLEAR;
        tinfo.store_op    = SDL_GPU_STOREOP_STORE;
        tinfo.clear_color = {0.45f, 0.55f, 0.60f, 1.00f};

        gContext.renderData.projectPass = SDL_BeginGPURenderPass(cmdBuf, &tinfo, 1, nullptr);

        SDL_BindGPUGraphicsPipeline(gContext.renderData.projectPass, RenderPipeline);
        SDL_BindGPUVertexStorageBuffers(gContext.renderData.projectPass, 0, &SpriteDataBuffer, 1);

        SDL_GPUTextureSamplerBinding binding {};
        binding.texture = Texture;
        binding.sampler = Sampler;

        SDL_BindGPUFragmentSamplers(gContext.renderData.projectPass, 0, &binding, 1);
        SDL_PushGPUVertexUniformData(cmdBuf, 0, &cameraMatrix, sizeof(Matrix4x4));

        SDL_DrawGPUPrimitives(gContext.renderData.projectPass, SPRITE_COUNT * 6, 1, 0, 0);

        SDL_EndGPURenderPass(gContext.renderData.projectPass);
    }

    SDL_SubmitGPUCommandBuffer(cmdBuf);
    return true;
}

void Boids::Quit()
{
    SDL_ReleaseGPUGraphicsPipeline(gContext.renderData.device, RenderPipeline);
    SDL_ReleaseGPUSampler(gContext.renderData.device, Sampler);
    SDL_ReleaseGPUTexture(gContext.renderData.device, Texture);
    SDL_ReleaseGPUTransferBuffer(gContext.renderData.device, SpriteDataTransferBuffer);
    SDL_ReleaseGPUBuffer(gContext.renderData.device, SpriteDataBuffer);
}

bool Boids::DrawUI()
{
    return true;
}
