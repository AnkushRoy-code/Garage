#include "EventHandler.h"
#include "Core/Context.h"
#include "Common/BaseProject.h"
#include "SDL3/SDL_init.h"

void changeState();
SDL_AppResult Core::EventHandler::handleEvents(SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        if (gContext.projectIndex != -1) { Projects[gContext.projectIndex]->Quit(); }
        return SDL_APP_SUCCESS;
    }

    else if (event->type == SDL_EVENT_KEY_DOWN)
    {
        switch (event->key.scancode)
        {
            case SDL_SCANCODE_D:
            {
                gContext.changeState = true;
                gContext.pressedD    = true;
                break;
            };

            case SDL_SCANCODE_A:
            {
                gContext.changeState = true;
                gContext.pressedA    = true;
                break;
            };

            case SDL_SCANCODE_LEFT: gContext.left = true; break;
            case SDL_SCANCODE_RIGHT: gContext.right = true; break;
            case SDL_SCANCODE_DOWN: gContext.down = true; break;
            case SDL_SCANCODE_UP: gContext.up = true; break;

            default: break;
        }
    }

    else if (event->type == SDL_EVENT_KEY_UP)
    {
        switch (event->key.scancode)
        {
            case SDL_SCANCODE_LEFT: gContext.left = false; break;
            case SDL_SCANCODE_RIGHT: gContext.right = false; break;
            case SDL_SCANCODE_DOWN: gContext.down = false; break;
            case SDL_SCANCODE_UP: gContext.up = false; break;
            default: break;
        }
    }

    else if (event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        SDL_GetWindowSize(gContext.mWindow, &gContext.width, &gContext.height);
        io.DisplaySize = ImVec2((float)gContext.width, (float)gContext.height);
    }

    // Change Project
    changeState();

    return SDL_APP_CONTINUE;
}

void changeState()
{
    if (gContext.changeState)
    {
        if (!Projects.empty() && gContext.projectIndex < Projects.size())
        {
            const size_t newIndex =
                gContext.pressedA ? std::max(int(0), gContext.projectIndex - 1)
                                  : std::min(gContext.projectIndex + 1, int(Projects.size()) - 1);
            if (newIndex != gContext.projectIndex)
            {
                Projects[gContext.projectIndex]->Quit();
                gContext.projectIndex = newIndex;
                Projects[gContext.projectIndex]->Init();
            }
        }
        gContext.changeState = false;
        gContext.pressedA    = false;
        gContext.pressedD    = false;  // I don't even use this variable...
    }
}
