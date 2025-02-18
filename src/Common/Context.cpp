#include "Context.h"
#include <SDL3/SDL_render.h>
#include <print>

bool Context::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::println("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    SDL_CreateWindowAndRenderer("SDL_GPU_API_TRY", 640, 480, 0, &mWindow, &mRenderer);

    // clang-format off
    if (!mWindow) { std::println("Unable to create SDL_Window. SDL_Error: %s", SDL_GetError()); }
    if (!mRenderer) { std::println("Unable to create SDL_Renderer. SDL_Error: %s", SDL_GetError()); }
    // clang-format on

    return true;
}

std::unique_ptr<std::vector<Example>> ExampleRegistry::instances = nullptr;

const std::vector<Example> &ExampleRegistry::getAllExamples()
{
    if (!instances) { instances = std::make_unique<std::vector<Example>>(); }
    return *instances;
}

void ExampleRegistry::registerExample(const std::string &name,
                                      std::function<bool(Context &)> init,
                                      std::function<bool(Context &)> update,
                                      std::function<bool(Context &)> draw,
                                      std::function<void(Context &)> quit)
{
    if (!instances) instances = std::make_unique<std::vector<Example>>();
    instances->push_back({name, init, update, draw, quit});
}
