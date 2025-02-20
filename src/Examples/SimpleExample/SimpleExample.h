#ifndef INCLUDE_SIMPLEEXAMPLE_SIMPLEEXAMPLE_H_
#define INCLUDE_SIMPLEEXAMPLE_SIMPLEEXAMPLE_H_

#include "Common/Context.h"
#include "Common/SDL_Exception.h"
#include <print>

/**
 * @namespace SimpleExample
 * @brief A basic example/template for structuring graphics-related code.
 *
 * This example provides a minimal structure that all other examples should follow.
 * Every function is mandatory, but they can be left empty if not required.
 */
namespace SimpleExample
{

/**
 * @brief Initializes the example.
 *
 * This function runs once at the start and is used to set up the necessary resources.
 *
 * @param context Reference to the global Context object.
 * @return true if initialization succeeds, false otherwise.
 * @throws SDL_Exception if an SDL operation fails.
 */
static bool Init(Context &context)
{
    std::println("SimpleExample Initialised");
    return true;
}

/**
 * @brief Updates the example.
 *
 * This function is called every frame before rendering. It is typically used to
 * update game logic, process input, and prepare data for rendering.
 *
 * @param context Reference to the global Context object.
 * @return true if the update succeeds, false otherwise.
 * @throws SDL_Exception if an SDL operation fails.
 */
static bool Update(Context &context)
{
    return true;
}

/**
 * @brief Renders the example.
 *
 * This function is called every frame after updating. It handles all the rendering
 * operations, including acquiring a GPU command buffer, clearing the screen, and
 * submitting the frame to the GPU.
 *
 * @param context Reference to the global Context object.
 * @return true if rendering succeeds, false otherwise.
 * @throws SDL_Exception if an SDL operation fails.
 */
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

/**
 * @brief Cleans up resources.
 *
 * This function is called once during shutdown. It is used to free any allocated
 * resources, close files, and perform necessary cleanup.
 *
 * @param context Reference to the global Context object.
 * @throws SDL_Exception if an SDL operation fails.
 */
static void Quit(Context &context) {}

}  // namespace SimpleExample

/**
 * @brief Registers the SimpleExample in the ExampleRegistry.
 *
 * @return true if registration succeeds, false otherwise.
 */
bool registerSimpleExample();

#endif  // INCLUDE_SIMPLEEXAMPLE_SIMPLEEXAMPLE_H_
