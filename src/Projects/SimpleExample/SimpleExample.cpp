#include "SimpleExample.h"

bool SimpleExample::Init(Context &context)
{

    std::cout << "SimpleExample Initialised\n";
    return true;
}
bool SimpleExample::Update(Context &context) 
{
    return true;
}

bool SimpleExample::Draw(Context &context) 
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
        colorTargetInfo.clear_color = SDL_FColor {0.1f, 0.7f, 0.8f, 1.0f};  // Light cyan background
        colorTargetInfo.load_op     = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op    = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass *renderPass =
            SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, nullptr);
        SDL_EndGPURenderPass(renderPass);
    }

    SDL_SubmitGPUCommandBuffer(cmdbuf);
    return true;
}

void SimpleExample::Quit(Context &context)  {}
