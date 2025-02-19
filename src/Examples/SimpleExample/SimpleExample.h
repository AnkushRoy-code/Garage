#ifndef INCLUDE_SIMPLEEXAMPLE_SIMPLEEXAMPLE_H_
#define INCLUDE_SIMPLEEXAMPLE_SIMPLEEXAMPLE_H_

#include "Common/Context.h"
#include "Common/SDL_Exception.h"
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
    if (cmdbuf == nullptr) { throw SDL_Exception("AcquireGPUCommandBuffer failed"); }

    SDL_GPUTexture *swapchainTexture;
    if (!SDL_AcquireGPUSwapchainTexture(cmdbuf, context.mWindow, &swapchainTexture, nullptr,
                                        nullptr))
    {
        throw SDL_Exception("WaitAndAcquireGPUSwapchainTexture failed");
    }

    if (swapchainTexture != nullptr)
    {
        SDL_GPUColorTargetInfo colorTargetInfo = {nullptr};
        colorTargetInfo.texture                = swapchainTexture;
        colorTargetInfo.clear_color            = (SDL_FColor) {0.1f, 0.7f, 0.8f, 1.0f};
        colorTargetInfo.load_op                = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op               = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass *renderPass =
            SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, nullptr);
        SDL_EndGPURenderPass(renderPass);
    }

    SDL_SubmitGPUCommandBuffer(cmdbuf);
    return true;
}

static void Quit(Context &context) {}

}  // namespace SimpleExample

bool registerSimpleExample();

#endif  // INCLUDE_SIMPLEEXAMPLE_SIMPLEEXAMPLE_H_
