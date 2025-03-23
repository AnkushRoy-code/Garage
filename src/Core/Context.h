#ifndef INCLUDE_CORE_CONTEXT_H_
#define INCLUDE_CORE_CONTEXT_H_

#include <SDL3/SDL.h>
#include <imgui.h>

namespace Core
{

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
    SDL_Window *window {};     ///< Pointer to the SDL window instance.
    SDL_GPUDevice *device {};  ///< Pointer to the SDL GPU device instance.
    SDL_GPURenderPass *projectPass {};
    SDL_GPUTexture *projectTexture {};
    SDL_GPUSampler *projectSampler {};

    // I might have these in a different bitset later on.
    bool right {};  ///< True if the right key is pressed
    bool left {};   ///< True if the left key is pressed
    bool down {};   ///< True if the down key is pressed
    bool up {};     ///< True if the up key is pressed
    bool pressedA {};
    bool pressedD {};
    bool changeState {};  ///< True if a or d was pressed an the state needs to change.

    ImGuiID mainViewportId {};

    int width {1280};
    int height {720};

    int projectIndex {};
};

}  // namespace Core

extern Core::Context gContext;

#endif  // INCLUDE_CORE_CONTEXT_H_
