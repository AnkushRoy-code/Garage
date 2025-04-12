#include "Core/EventHandler.h"

#include "Core/Context.h"
#include "Projects/Common/BaseProject.h"

Core::EventHandlerStruct::EventHandlerStruct()
{
    Keys.fill(KEY_STATE::IDLE);
}

bool Core::EventHandlerStruct::GetEventHeld(KEY p_Key)
{
    return (Keys[p_Key] == Core::KEY_STATE::PRESSED || Keys[p_Key] == Core::KEY_STATE::HELD);
}

bool Core::EventHandlerStruct::GetEventPressed(KEY p_Key)
{
    return (Keys[p_Key] == Core::KEY_STATE::PRESSED);
}

void Core::EventHandlerStruct::UpdateKey(KEY p_Action, bool p_Pressed)
{
    const auto index = static_cast<std::size_t>(p_Action);
    if (p_Pressed)
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

void Core::EventHandlerStruct::EndFrame()
{
    for (auto &state: Keys)
    {
        if (state == KEY_STATE::PRESSED)
            state = KEY_STATE::HELD;
        else if (state == KEY_STATE::RELEASED)
            state = KEY_STATE::IDLE;
    }

    // special case of mouse roll (one time thingy)
    if (Keys[KEY::MOUSE_ROLL] == KEY_STATE::PRESSED || Keys[KEY::MOUSE_ROLL] == KEY_STATE::HELD)
    {
        Keys[KEY::MOUSE_ROLL] = KEY_STATE::IDLE;
    }
}

SDL_AppResult Core::EventHandlerStruct::HandleEvents(SDL_Event *p_Event)
{
    if (p_Event->type == SDL_EVENT_QUIT)
    {
        if (g_Context.AppState.ProjectIndex != -1)
        {
            g_Projects[g_Context.AppState.ProjectIndex]->Quit();
        }
        return SDL_APP_SUCCESS;
    }

    else if (p_Event->type == SDL_EVENT_KEY_DOWN || p_Event->type == SDL_EVENT_KEY_UP)
    {
        bool pressed = (p_Event->type == SDL_EVENT_KEY_DOWN);
        switch (p_Event->key.scancode)
        {
            case SDL_SCANCODE_RIGHT: UpdateKey(KEY::RIGHT, pressed); break;
            case SDL_SCANCODE_LEFT: UpdateKey(KEY::LEFT, pressed); break;
            case SDL_SCANCODE_DOWN: UpdateKey(KEY::DOWN, pressed); break;
            case SDL_SCANCODE_UP: UpdateKey(KEY::UP, pressed); break;
            case SDL_SCANCODE_W: UpdateKey(KEY::W, pressed); break;
            case SDL_SCANCODE_A: UpdateKey(KEY::A, pressed); break;
            case SDL_SCANCODE_S: UpdateKey(KEY::S, pressed); break;
            case SDL_SCANCODE_D: UpdateKey(KEY::D, pressed); break;
            default: break;
        }
    }

    else if (p_Event->type == SDL_EVENT_MOUSE_BUTTON_DOWN
             || p_Event->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        bool pressed = (p_Event->type == SDL_EVENT_MOUSE_BUTTON_DOWN);
        switch (p_Event->button.button)
        {
            case SDL_BUTTON_LEFT: UpdateKey(KEY::MOUSE_LEFT_CLICK, pressed); break;
            case SDL_BUTTON_RIGHT: UpdateKey(KEY::MOUSE_RIGHT_CLICK, pressed); break;
            case SDL_BUTTON_MIDDLE: UpdateKey(KEY::MOUSE_MIDDLE_CLICK, pressed); break;
            default: break;
        }
    }

    else if (p_Event->type == SDL_EVENT_MOUSE_WHEEL)
    {
        UpdateKey(KEY::MOUSE_ROLL, true);
        g_Context.AppState.HorizontalScroll = p_Event->wheel.x;
        g_Context.AppState.VerticalScroll   = p_Event->wheel.y;
    }

    else if (p_Event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        UpdateKey(KEY::RESIZE_PROJECT_WINDOW, true);

        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        SDL_GetWindowSize(g_Context.RenderData.Window, &g_Context.RenderData.Width,
                          &g_Context.RenderData.Height);
        io.DisplaySize =
            ImVec2((float)g_Context.RenderData.Width, (float)g_Context.RenderData.Height);
    }

    return SDL_APP_CONTINUE;
}
