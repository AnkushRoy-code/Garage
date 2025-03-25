#include "EventHandler.h"
#include "Core/Context.h"
#include "Common/BaseProject.h"

#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL.h>

Core::InputHandler::InputHandler()
{
    Keys.fill(KEY_STATE::IDLE);
}

void Core::InputHandler::updateKey(KEY action, bool pressed)
{
    auto index = static_cast<std::size_t>(action);
    if (pressed)
    {
        if (Keys[index] == KEY_STATE::IDLE || Keys[index] == KEY_STATE::RELEASED)
        {
            Keys[index] = KEY_STATE::PRESSED;
        }
        else { Keys[index] = KEY_STATE::HELD; }
    }
    else
    {
        if (Keys[index] == KEY_STATE::PRESSED || Keys[index] == KEY_STATE::HELD)
        {
            Keys[index] = KEY_STATE::RELEASED;
        }
        else { Keys[index] = KEY_STATE::IDLE; }
    }
}

void Core::InputHandler::endFrame()
{
    for (auto &state: Keys)
    {
        if (state == KEY_STATE::PRESSED)
            state = KEY_STATE::HELD;
        else if (state == KEY_STATE::RELEASED)
            state = KEY_STATE::IDLE;
    }
}

SDL_AppResult Core::EventHandler::handleEvents(SDL_Event *event, InputHandler &inputHandler)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        if (gContext.appState.projectIndex != -1)
        {
            Projects[gContext.appState.projectIndex]->Quit();
        }
        return SDL_APP_SUCCESS;
    }

    else if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP)
    {
        bool pressed = (event->type == SDL_EVENT_KEY_DOWN);
        switch (event->key.scancode)
        {
            case SDL_SCANCODE_RIGHT: inputHandler.updateKey(KEY::RIGHT, pressed); break;
            case SDL_SCANCODE_LEFT: inputHandler.updateKey(KEY::LEFT, pressed); break;
            case SDL_SCANCODE_DOWN: inputHandler.updateKey(KEY::DOWN, pressed); break;
            case SDL_SCANCODE_UP: inputHandler.updateKey(KEY::UP, pressed); break;
            case SDL_SCANCODE_A: inputHandler.updateKey(KEY::A, pressed); break;
            case SDL_SCANCODE_D: inputHandler.updateKey(KEY::D, pressed); break;
            default: break;
        }
    }

    else if (event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        SDL_GetWindowSize(gContext.renderData.window, &gContext.renderData.width,
                          &gContext.renderData.height);
        io.DisplaySize =
            ImVec2((float)gContext.renderData.width, (float)gContext.renderData.height);
    }

    return SDL_APP_CONTINUE;
}
