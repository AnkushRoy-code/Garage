#include "Renderer.h"

#include "Core/Context.h"
#include "Common/BaseProject.h"
#include "Common/SDL_Exception.h"
#include "SDL3/SDL_gpu.h"

void Core::Renderer::Init()
{
    SDL_SetGPUSwapchainParameters(gContext.renderData.device, gContext.renderData.window,
                                  SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_MAILBOX);

    const auto format =
        SDL_GetGPUSwapchainTextureFormat(gContext.renderData.device, gContext.renderData.window);

    // detect the number of sample_count. highst is 8 but we can work with 4
    if (SDL_GPUTextureSupportsSampleCount(gContext.renderData.device, format,
                                          SDL_GPU_SAMPLECOUNT_4))
    {
        gContext.renderData.sampleCount = SDL_GPU_SAMPLECOUNT_4;
    }
    else if (SDL_GPUTextureSupportsSampleCount(gContext.renderData.device, format,
                                               SDL_GPU_SAMPLECOUNT_2))
    {
        gContext.renderData.sampleCount = SDL_GPU_SAMPLECOUNT_2;
    }
    else if (SDL_GPUTextureSupportsSampleCount(gContext.renderData.device, format,
                                               SDL_GPU_SAMPLECOUNT_1))
    {
        gContext.renderData.sampleCount = SDL_GPU_SAMPLECOUNT_1;
    }

    const SDL_GPUTextureCreateInfo gpuTextureCreateInfo = {
        .type                 = SDL_GPU_TEXTURETYPE_2D,
        .format               = format,
        .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        .width                = 640,
        .height               = 480,
        .layer_count_or_depth = 1,
        .num_levels           = 1,
        .sample_count         = gContext.renderData.sampleCount,
    };

    gContext.renderData.projectTexture =
        SDL_CreateGPUTexture(gContext.renderData.device, &gpuTextureCreateInfo);

    if (!gContext.renderData.projectTexture)
    {
        throw SDL_Exception("Unable to create GPU Texture");
    }

    const SDL_GPUTextureCreateInfo resolveTextureInfo {.type   = SDL_GPU_TEXTURETYPE_2D,
                                                       .format = format,
                                                       .usage  = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET
                                                                | SDL_GPU_TEXTUREUSAGE_SAMPLER,
                                                       .width                = 640,
                                                       .height               = 480,
                                                       .layer_count_or_depth = 1,
                                                       .num_levels           = 1};

    gContext.renderData.resolveTexture =
        SDL_CreateGPUTexture(gContext.renderData.device, &resolveTextureInfo);

    if (!gContext.renderData.resolveTexture)
    {
        throw SDL_Exception("Unable to create resolve Texture");
    }

    const SDL_GPUSamplerCreateInfo samplerInfo {
        .min_filter     = SDL_GPU_FILTER_NEAREST,
        .mag_filter     = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode    = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    };

    gContext.renderData.projectSampler =
        SDL_CreateGPUSampler(gContext.renderData.device, &samplerInfo);
}

void Core::Renderer::ResizeProjectTexture(int w, int h)
{
    SDL_ReleaseGPUTexture(gContext.renderData.device, gContext.renderData.projectTexture);
    SDL_ReleaseGPUTexture(gContext.renderData.device, gContext.renderData.resolveTexture);

    const auto format =
        SDL_GetGPUSwapchainTextureFormat(gContext.renderData.device, gContext.renderData.window);

    const SDL_GPUTextureCreateInfo gpuTextureCreateInfo = {
        .type                 = SDL_GPU_TEXTURETYPE_2D,
        .format               = format,
        .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        .width                = (Uint32)w,
        .height               = (Uint32)h,
        .layer_count_or_depth = 1,
        .num_levels           = 1,
        .sample_count         = gContext.renderData.sampleCount,
    };

    gContext.renderData.projectTexture =
        SDL_CreateGPUTexture(gContext.renderData.device, &gpuTextureCreateInfo);

    const SDL_GPUTextureCreateInfo resolveTextureInfo = {
        .type                 = SDL_GPU_TEXTURETYPE_2D,
        .format               = format,
        .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        .width                = (Uint32)w,
        .height               = (Uint32)h,
        .layer_count_or_depth = 1,
        .num_levels           = 1};

    gContext.renderData.resolveTexture =
        SDL_CreateGPUTexture(gContext.renderData.device, &resolveTextureInfo);
}

void Core::Renderer::DrawProjectToTexture()
{
    Projects[gContext.appState.projectIndex]->Draw();
}
