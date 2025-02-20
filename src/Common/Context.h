#ifndef INCLUDE_COMMON_CONTEXT_H_
#define INCLUDE_COMMON_CONTEXT_H_

#include <SDL3/SDL.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>

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

    SDL_Window *mWindow {};  ///< Pointer to the SDL window instance.
    SDL_GPUDevice *mDevice {}; ///< Pointer to the SDL GPU device instance.

    // Boolean variables representing key states for movement input
    bool right {}; ///< True if the right key is pressed
    bool left {};  ///< True if the left key is pressed
    bool down {};  ///< True if the down key is pressed
    bool up {};    ///< True if the up key is pressed
};

/**
 * @brief Structure representing an example application instance.
 * 
 * This structure contains function callbacks required for initialization,
 * updating, drawing, and quitting an example.
 */
struct Example
{
    std::string Name; ///< Name of the example.
    
    std::function<bool(Context &Context)> Init; ///< Function to initialize the example.
    std::function<bool(Context &Context)> Update; ///< Function to update the example logic.
    std::function<bool(Context &Context)> Draw; ///< Function to render the example.
    std::function<void(Context &Context)> Quit; ///< Function to clean up the example.
};

/**
 * @brief Class to register and manage example applications.
 * 
 * This class provides static methods for registering examples
 * and retrieving the list of registered examples.
 */
class ExampleRegistry
{
  public:
    /**
     * @brief Registers a new example with its associated functions.
     * 
     * @param name Name of the example.
     * @param init Function to initialize the example.
     * @param update Function to update the example.
     * @param draw Function to draw the example.
     * @param quit Function to clean up the example.
     */
    static void registerExample(const std::string &name,
                                std::function<bool(Context &)> init,
                                std::function<bool(Context &)> update,
                                std::function<bool(Context &)> draw,
                                std::function<void(Context &)> quit);

    /**
     * @brief Retrieves all registered examples.
     * 
     * @return A constant reference to a vector of registered examples.
     */
    static const std::vector<Example> &getAllExamples();

    /**
     * @brief Registers all predefined examples.
     * 
     * This function is defined in `RegisterExample.cpp` and is responsible for
     * calling `registerExample` to add various predefined examples.
     * 
     * @return True if registration is successful, false otherwise.
     */
    static bool registerExamples();

  private:
    static std::unique_ptr<std::vector<Example>> instances; ///< Stores the registered examples.
};

#endif  // INCLUDE_COMMON_CONTEXT_H_
