#include "../../Common/Context.h"
#include <print>

namespace SimpleExample
{

static bool Init(Context &context)
{
    std::println("SimpleExample Initialised");
    return true;
}

static bool Update(Context &context)
{
    return true;
}

static bool Draw(Context &context)
{
    SDL_GPUCommandBuffer *cmdbuf = SDL_AcquireGPUCommandBuffer(context.mDevice);
    if (cmdbuf == NULL)
    {
        SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
        return -1;
    }

    SDL_GPUTexture *swapchainTexture;
    if (!SDL_AcquireGPUSwapchainTexture(cmdbuf, context.mWindow, &swapchainTexture, NULL, NULL))
    {
        SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
        return -1;
    }

    if (swapchainTexture != NULL)
    {
        SDL_GPUColorTargetInfo colorTargetInfo = {0};
        colorTargetInfo.texture                = swapchainTexture;
        colorTargetInfo.clear_color            = (SDL_FColor) {0.1f, 0.7f, 0.8f, 1.0f};
        colorTargetInfo.load_op                = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op               = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, NULL);
        SDL_EndGPURenderPass(renderPass);
    }

    SDL_SubmitGPUCommandBuffer(cmdbuf);
    return true;
    return true;
}

static void Quit(Context &context) {}

}  // namespace SimpleExample

bool registerSimpleExample();
