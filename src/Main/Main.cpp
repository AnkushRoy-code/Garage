#include <SDL3/SDL_init.h>
#include <iostream>
#include <vector>
#include <algorithm>

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Common/Context.h"
#include "Utils/Time.h"
#include "Common/SDL_Exception.h"

Context context;
std::vector<Example> Examples;
int exampleIndex {};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    try
    {
        context.init();

        ExampleRegistry::registerExamples();
        Examples = ExampleRegistry::getAllExamples();

        // std::cout << "The Example name is: " << Examples[0].Name << std::endl;
        // Manually init the first example
        Examples[0].Init(context);  // Error handling can wait

        Utils::Time::init();
    }

    catch (const SDL_Exception &e)
    {
        std::cerr << "Error in SDL_AppInit()\n" << e.what();
        return SDL_APP_FAILURE;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error something went wrong while SDL_AppInit()\n" << e.what();
        return SDL_APP_FAILURE;
    }

    catch (...)
    {
        std::cerr << "Some unknown error occured inside SDL_AppInit() function\n";
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    try
    {
        Utils::Time::updateDeltaTime();

        bool changeState {false};
        bool Pressed_A {false};  // to detect if A was pressed or D was pressed. To properly Quit
                                 // the running example.

        if (event->type == SDL_EVENT_QUIT)
        {
            if (exampleIndex != -1) { Examples[exampleIndex].Quit(context); }
            return SDL_APP_SUCCESS;
        }

        else if (event->type == SDL_EVENT_KEY_DOWN)
        {
            switch (event->key.key)
            {
                case SDLK_D:
                {
                    changeState = true;
                    Pressed_A   = false;
                    break;
                };

                case SDLK_A:
                {
                    changeState = true;
                    Pressed_A   = true;
                    break;
                };

                case SDLK_LEFT: context.left = true; break;
                case SDLK_RIGHT: context.right = true; break;
                case SDLK_DOWN: context.down = true; break;
                case SDLK_UP: context.up = true; break;

                default: break;
            }
        }

        // Change Example
        if (changeState)
        {
            if (!Examples.empty() && exampleIndex < Examples.size())
            {
                // Calculate target index
                size_t newIndex = Pressed_A ? std::max(int(0), exampleIndex - 1)
                                            : std::min(exampleIndex + 1, int(Examples.size()) - 1);
                // Perform transition
                if (newIndex != exampleIndex)
                {
                    Examples[exampleIndex].Quit(context);
                    exampleIndex = newIndex;
                    Examples[exampleIndex].Init(context);
                }
            }
        }
    }

    catch (const SDL_Exception &e)
    {
        std::cerr << "Error in SDL_AppEvent()\n" << e.what();
        return SDL_APP_FAILURE;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error something went wrong while SDL_AppEvent()\n" << e.what();
        return SDL_APP_FAILURE;
    }

    catch (...)
    {
        std::cerr << "Some unknown error occured inside SDL_AppEvent() function\n";
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    try
    {
        Examples[exampleIndex].Update(context);
        Examples[exampleIndex].Draw(context);

        Utils::Time::capFPS();
    }

    catch (const SDL_Exception &e)
    {
        std::cerr << "Error in SDL_AppIterate()\n" << e.what();
        return SDL_APP_FAILURE;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error something went wrong while SDL_AppIterate()\n" << e.what();
        return SDL_APP_FAILURE;
    }

    catch (...)
    {
        std::cerr << "Some unknown error occured inside SDL_AppIterate() function\n";
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // Do stuff when needed
}
