#ifndef INCLUDE_SIMPLEEXAMPLE_SIMPLEEXAMPLE_H_
#define INCLUDE_SIMPLEEXAMPLE_SIMPLEEXAMPLE_H_

#include "Common/BaseProject.h"

/**
 * @namespace SimpleExample
 * @brief A basic example/template for structuring graphics-related code.
 *
 * This example provides a minimal structure that all other examples should follow.
 * Every function is mandatory, but they can be left empty if not required.
 */
class SimpleExample : public Common::BaseProject
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
    bool Init() override;

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
    bool Update() override;

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
    bool Draw() override;

    /**
     * @brief Cleans up resources.
     *
     * This function is called once during shutdown. It is used to free any allocated
     * resources, close files, and perform necessary cleanup.
     *
     * @param context Reference to the global Context object.
     * @throws SDL_Exception if an SDL operation fails.
     */
    void Quit() override;

    
    std::string name = "SimpleExample";
    [[nodiscard]] const std::string &getName() override {
        return name;
    }
};

#endif  // INCLUDE_SIMPLEEXAMPLE_SIMPLEEXAMPLE_H_
