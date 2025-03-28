#include "Renderer.h"

#include "Core/Context.h"
#include "Common/BaseProject.h"
#include "Common/SDL_Exception.h"

void Core::Renderer::Init()
{
    SDL_SetGPUSwapchainParameters(gContext.renderData.device, gContext.renderData.window,
                                  SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_MAILBOX);

    SDL_GPUTextureCreateInfo gpuTextureCreateInfo = {};
    gpuTextureCreateInfo.type                     = SDL_GPU_TEXTURETYPE_2D;
    gpuTextureCreateInfo.format                   = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    gpuTextureCreateInfo.usage  = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;
    gpuTextureCreateInfo.width  = 640;
    gpuTextureCreateInfo.height = 480;
    gpuTextureCreateInfo.layer_count_or_depth = 1;
    gpuTextureCreateInfo.num_levels           = 1;
    gpuTextureCreateInfo.sample_count         = SDL_GPU_SAMPLECOUNT_1;

    gContext.renderData.projectTexture =
        SDL_CreateGPUTexture(gContext.renderData.device, &gpuTextureCreateInfo);

    if (!gContext.renderData.projectTexture)
    {
        throw SDL_Exception("Unable to create GPU Texture");
    }

    SDL_GPUSamplerCreateInfo samplerInfo {};
    samplerInfo.min_filter     = SDL_GPU_FILTER_NEAREST,
    samplerInfo.mag_filter     = SDL_GPU_FILTER_NEAREST,
    samplerInfo.mipmap_mode    = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
    samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,

    gContext.renderData.projectSampler =
        SDL_CreateGPUSampler(gContext.renderData.device, &samplerInfo);
}

void Core::Renderer::DrawProjectToTexture()
{
    Projects[gContext.appState.projectIndex]->Draw();
}
