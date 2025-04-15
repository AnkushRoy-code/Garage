#include "Core/Renderer/Renderer.h"

#include "Core/Context.h"
#include "Core/Console.h"
#include "Projects/Common/BaseProject.h"

#include "Core/Common/pch.h"
#include <iostream>

void Core::Renderer::Init()
{
    auto &rndt = Core::Context::GetContext()->RenderData;

    SDL_SetGPUSwapchainParameters(rndt.Device, rndt.Window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                                  SDL_GPU_PRESENTMODE_MAILBOX);

    const auto format = SDL_GetGPUSwapchainTextureFormat(rndt.Device, rndt.Window);

    // detect the number of sample_count. highst is 8 but we can work with 4
#ifdef _WIN32  // could'nt get anti-aliasing working with Windows.
    rndt.SampleCount = SDL_GPU_SAMPLECOUNT_1;
#else
    if (SDL_GPUTextureSupportsSampleCount(rndt.Device, format, SDL_GPU_SAMPLECOUNT_4))
    {
        rndt.SampleCount = SDL_GPU_SAMPLECOUNT_4;
    }
    else if (SDL_GPUTextureSupportsSampleCount(rndt.Device, format, SDL_GPU_SAMPLECOUNT_2))
    {
        rndt.SampleCount = SDL_GPU_SAMPLECOUNT_2;
    }
    else if (SDL_GPUTextureSupportsSampleCount(rndt.Device, format, SDL_GPU_SAMPLECOUNT_1))
    {
        rndt.SampleCount = SDL_GPU_SAMPLECOUNT_1;
    }
#endif

    const SDL_GPUTextureCreateInfo gpuTextureCreateInfo = {
        .type                 = SDL_GPU_TEXTURETYPE_2D,
        .format               = format,
        .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        .width                = 640,
        .height               = 480,
        .layer_count_or_depth = 1,
        .num_levels           = 1,
        .sample_count         = rndt.SampleCount,
    };

    rndt.ProjectTexture = SDL_CreateGPUTexture(rndt.Device, &gpuTextureCreateInfo);
    assert(rndt.ProjectTexture);

    const SDL_GPUTextureCreateInfo resolveTextureInfo {.type   = SDL_GPU_TEXTURETYPE_2D,
                                                       .format = format,
                                                       .usage  = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET
                                                                | SDL_GPU_TEXTUREUSAGE_SAMPLER,
                                                       .width                = 640,
                                                       .height               = 480,
                                                       .layer_count_or_depth = 1,
                                                       .num_levels           = 1};

    rndt.ResolveTexture = SDL_CreateGPUTexture(rndt.Device, &resolveTextureInfo);
    assert(rndt.ResolveTexture);

    const SDL_GPUSamplerCreateInfo samplerInfo {
        .min_filter     = SDL_GPU_FILTER_NEAREST,
        .mag_filter     = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode    = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    };

    rndt.ProjectSampler = SDL_CreateGPUSampler(rndt.Device, &samplerInfo);
    assert(rndt.ProjectSampler);
}

void Core::Renderer::ResizeProjectTexture(int p_Width, int p_Height)
{
    auto &rndt = Core::Context::GetContext()->RenderData;

    SDL_ReleaseGPUTexture(rndt.Device, rndt.ProjectTexture);
    SDL_ReleaseGPUTexture(rndt.Device, rndt.ResolveTexture);

    const auto format = SDL_GetGPUSwapchainTextureFormat(rndt.Device, rndt.Window);

    const SDL_GPUTextureCreateInfo gpuTextureCreateInfo = {
        .type                 = SDL_GPU_TEXTURETYPE_2D,
        .format               = format,
        .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        .width                = (Uint32)(p_Width * rndt.ResolutionScale),
        .height               = (Uint32)(p_Height * rndt.ResolutionScale),
        .layer_count_or_depth = 1,
        .num_levels           = 1,
        .sample_count         = rndt.SampleCount,
    };

    rndt.ProjectTexture = SDL_CreateGPUTexture(rndt.Device, &gpuTextureCreateInfo);
    assert(rndt.ProjectTexture);

    const SDL_GPUTextureCreateInfo resolveTextureInfo = {
        .type                 = SDL_GPU_TEXTURETYPE_2D,
        .format               = format,
        .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        .width                = (Uint32)(p_Width * rndt.ResolutionScale),
        .height               = (Uint32)(p_Height * rndt.ResolutionScale),
        .layer_count_or_depth = 1,
        .num_levels           = 1};

    rndt.ResolveTexture = SDL_CreateGPUTexture(rndt.Device, &resolveTextureInfo);
    assert(rndt.ResolveTexture);
}

void Core::Renderer::DrawProjectToTexture()
{
    Common::ProjectManager::GetProjects()
        ->at(Core::Context::GetContext()->AppState.ProjectIndex)
        ->Draw();
}
