#ifndef INCLUDE_COMMON_CONTEXT_H_
#define INCLUDE_COMMON_CONTEXT_H_

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <functional>
#include <memory>
#include <string>

struct Context
{
    Context()  = default;
    ~Context() = default;

    bool init();

    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;

    // Bools to get the input
    bool right {}, left {}, down {}, up {};
};

struct Example
{
    std::string Name;
    std::function<bool(Context &Context)> Init;
    std::function<bool(Context &Context)> Update;
    std::function<bool(Context &Context)> Draw;
    std::function<void(Context &Context)> Quit;
};

class ExampleRegistry
{
  public:
    static void registerExample(const std::string &name,
                                std::function<bool(Context &)> init,
                                std::function<bool(Context &)> update,
                                std::function<bool(Context &)> draw,
                                std::function<void(Context &)> quit);

    static const std::vector<Example> &getAllExamples();
    static bool registerExamples();  // This is defined in RegisterExample.cpp

  private:
    static std::unique_ptr<std::vector<Example>> instances;
};

#endif  // INCLUDE_COMMON_CONTEXT_H_
