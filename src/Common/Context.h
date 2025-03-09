#ifndef INCLUDE_COMMON_CONTEXT_H_
#define INCLUDE_COMMON_CONTEXT_H_

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

    SDL_Window *mWindow {};     ///< Pointer to the SDL window instance.
    SDL_GPUDevice *mDevice {};  ///< Pointer to the SDL GPU device instance.
    SDL_GPURenderPass *mRenderPass {};
    SDL_GPURenderPass *mProjectPass {};
    SDL_GPUTexture *mProjectTexture {};

    // Boolean variables representing key states for movement input
    bool right {};  ///< True if the right key is pressed
    bool left {};   ///< True if the left key is pressed
    bool down {};   ///< True if the down key is pressed
    bool up {};     ///< True if the up key is pressed
};

#endif  // INCLUDE_COMMON_CONTEXT_H_
