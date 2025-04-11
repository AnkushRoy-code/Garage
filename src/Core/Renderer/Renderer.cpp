#include "Renderer.h"

#include "Core/Context.h"
#include "Projects/Common/BaseProject.h"
#include "Core/Common/SDL_Exception.h"

#include "Core/Common/pch.h"

void Core::Renderer::Init()
{
    SDL_SetGPUSwapchainParameters(g_Context.RenderData.Device, g_Context.RenderData.Window,
                                  SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_MAILBOX);

    const auto format =
        SDL_GetGPUSwapchainTextureFormat(g_Context.RenderData.Device, g_Context.RenderData.Window);

    // detect the number of sample_count. highst is 8 but we can work with 4
#ifdef _WIN32  // could'nt get anti-aliasing working with Windows.
    g_Context.RenderData.SampleCount = SDL_GPU_SAMPLECOUNT_1;
#else
    if (SDL_GPUTextureSupportsSampleCount(g_Context.RenderData.Device, format,
                                          SDL_GPU_SAMPLECOUNT_4))
    {
        g_Context.RenderData.SampleCount = SDL_GPU_SAMPLECOUNT_4;
    }
    else if (SDL_GPUTextureSupportsSampleCount(g_Context.RenderData.Device, format,
                                               SDL_GPU_SAMPLECOUNT_2))
    {
        g_Context.RenderData.SampleCount = SDL_GPU_SAMPLECOUNT_2;
    }
    else if (SDL_GPUTextureSupportsSampleCount(g_Context.RenderData.Device, format,
                                               SDL_GPU_SAMPLECOUNT_1))
    {
        g_Context.RenderData.SampleCount = SDL_GPU_SAMPLECOUNT_1;
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
        .sample_count         = g_Context.RenderData.SampleCount,
    };

    g_Context.RenderData.ProjectTexture =
        SDL_CreateGPUTexture(g_Context.RenderData.Device, &gpuTextureCreateInfo);

    if (!g_Context.RenderData.ProjectTexture)
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

    g_Context.RenderData.ResolveTexture =
        SDL_CreateGPUTexture(g_Context.RenderData.Device, &resolveTextureInfo);

    if (!g_Context.RenderData.ResolveTexture)
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

    g_Context.RenderData.ProjectSampler =
        SDL_CreateGPUSampler(g_Context.RenderData.Device, &samplerInfo);
}

void Core::Renderer::ResizeProjectTexture(int p_Width, int p_Height)
{
    SDL_ReleaseGPUTexture(g_Context.RenderData.Device, g_Context.RenderData.ProjectTexture);
    SDL_ReleaseGPUTexture(g_Context.RenderData.Device, g_Context.RenderData.ResolveTexture);

    const auto format =
        SDL_GetGPUSwapchainTextureFormat(g_Context.RenderData.Device, g_Context.RenderData.Window);

    const SDL_GPUTextureCreateInfo gpuTextureCreateInfo = {
        .type                 = SDL_GPU_TEXTURETYPE_2D,
        .format               = format,
        .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        .width                = (Uint32)(p_Width * g_Context.RenderData.ResolutionScale),
        .height               = (Uint32)(p_Height * g_Context.RenderData.ResolutionScale),
        .layer_count_or_depth = 1,
        .num_levels           = 1,
        .sample_count         = g_Context.RenderData.SampleCount,
    };

    g_Context.RenderData.ProjectTexture =
        SDL_CreateGPUTexture(g_Context.RenderData.Device, &gpuTextureCreateInfo);

    const SDL_GPUTextureCreateInfo resolveTextureInfo = {
        .type                 = SDL_GPU_TEXTURETYPE_2D,
        .format               = format,
        .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        .width                = (Uint32)(p_Width * g_Context.RenderData.ResolutionScale),
        .height               = (Uint32)(p_Height * g_Context.RenderData.ResolutionScale),
        .layer_count_or_depth = 1,
        .num_levels           = 1};

    g_Context.RenderData.ResolveTexture =
        SDL_CreateGPUTexture(g_Context.RenderData.Device, &resolveTextureInfo);
}

void Core::Renderer::DrawProjectToTexture()
{
    g_Projects[g_Context.AppState.ProjectIndex]->Draw();
}
