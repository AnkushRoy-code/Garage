#include "Renderer.h"

#include "Core/Context.h"
#include "Common/BaseProject.h"
#include "Common/SDL_Exception.h"

void Renderer::Init()
{
    SDL_SetGPUSwapchainParameters(gContext.mDevice, gContext.mWindow,
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

    gContext.mProjectTexture = SDL_CreateGPUTexture(gContext.mDevice, &gpuTextureCreateInfo);

    if (!gContext.mProjectTexture) { throw SDL_Exception("Unable to create GPU Texture"); }

    SDL_GPUSamplerCreateInfo samplerInfo {};
    samplerInfo.min_filter     = SDL_GPU_FILTER_NEAREST,
    samplerInfo.mag_filter     = SDL_GPU_FILTER_NEAREST,
    samplerInfo.mipmap_mode    = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
    samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,

    gContext.mProjectSampler = SDL_CreateGPUSampler(gContext.mDevice, &samplerInfo);
}

void Renderer::DrawToTex()
{
    SDL_GPUCommandBuffer *commandBufferProjects = SDL_AcquireGPUCommandBuffer(gContext.mDevice);
    if (!commandBufferProjects)
    {
        throw SDL_Exception("AcquireGPUCommandBuffer(Projects) failed!");
    }

    {
        const SDL_GPUColorTargetInfo projectTargetInfo {
            .texture     = gContext.mProjectTexture,
            .clear_color = SDL_FColor {0.45f, 0.55f, 0.60f, 1.00f},
            .load_op     = SDL_GPU_LOADOP_CLEAR,
            .store_op    = SDL_GPU_STOREOP_STORE,
            .cycle       = true,
        };

        gContext.mProjectPass =
            SDL_BeginGPURenderPass(commandBufferProjects, &projectTargetInfo, 1, nullptr);

        Projects[gContext.projectIndex]->Draw();

        SDL_EndGPURenderPass(gContext.mProjectPass);
        SDL_SubmitGPUCommandBuffer(commandBufferProjects);
    }
}
