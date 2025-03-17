#ifndef INCLUDE_CORE_CONTEXT_H_
#define INCLUDE_CORE_CONTEXT_H_

#include "imgui.h"
#include <SDL3/SDL.h>

/**
 * @brief Structure representing the application context.
 *
 * This structure holds essential components required for the
 * application, such as the SDL window and GPU device. Additionally,
 * it maintains boolean values to track user input.
 */
struct Context
{
    /**
     * @brief Default constructor for Context.
     */
    Context() = default;

    /**
     * @brief Destructor for Context.
     *
     * Ensures proper cleanup of allocated resources.
     */
    ~Context();

    /**
     * @brief Initializes the application context.
     *
     * This function sets up the SDL window and GPU device
     * necessary for rendering.
     */
    void init();

    // Data
    SDL_Window *mWindow {};     ///< Pointer to the SDL window instance.
    SDL_GPUDevice *mDevice {};  ///< Pointer to the SDL GPU device instance.
    SDL_GPURenderPass *mRenderPass {};
    SDL_GPURenderPass *mProjectPass {};
    SDL_GPUTexture *mProjectTexture {};
    SDL_GPUSampler *mProjectSampler {};

    // Boolean variables representing key states for movement input
    bool right {};  ///< True if the right key is pressed
    bool left {};   ///< True if the left key is pressed
    bool down {};   ///< True if the down key is pressed
    bool up {};     ///< True if the up key is pressed

    ImGuiID mainViewportId {};
};


extern Context gContext;

#endif  // INCLUDE_CORE_CONTEXT_H_
