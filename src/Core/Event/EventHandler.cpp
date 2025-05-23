#include "Core/Event/EventHandler.h"

#include "Core/Context.h"
#include "Projects/Common/BaseProject.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"

Core::EventHandlerStruct::EventHandlerStruct()
{
    Keys.fill(KEY_STATE::IDLE);
}

bool Core::EventHandlerStruct::GetEventHeld(KEY p_Key)
{
    // true if key is pressed or held
    return (Keys[p_Key] == Core::KEY_STATE::PRESSED || Keys[p_Key] == Core::KEY_STATE::HELD);
}

bool Core::EventHandlerStruct::GetEventPressed(KEY p_Key)
{
    // true if key is pressed
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
    if (GetEventHeld(Core::MOUSE_ROLL)) { Keys[KEY::MOUSE_ROLL] = KEY_STATE::IDLE; }
    if (GetEventHeld(Core::MOUSE_MOVE)) { Keys[KEY::MOUSE_MOVE] = KEY_STATE::IDLE; }
}

SDL_AppResult Core::EventHandlerStruct::HandleEvents(SDL_Event *p_Event)
{
    // why no SDL_PollEvents ..?
    // SDL_AppEvent() callback handles that for us

    if (p_Event->type == SDL_EVENT_QUIT)
    {
        if (Context::GetContext()->AppState.ProjectIndex != -1)
        {
            Common::ProjectManager::GetProjects()
                ->at(Context::GetContext()->AppState.ProjectIndex)
                ->Quit();
        }
        return SDL_APP_SUCCESS;
    }

    else if (p_Event->type == SDL_EVENT_KEY_DOWN || p_Event->type == SDL_EVENT_KEY_UP)
    {
        bool pressed = (p_Event->type == SDL_EVENT_KEY_DOWN);
        switch (p_Event->key.scancode)
        {
            case SDL_SCANCODE_ESCAPE: UpdateKey(KEY::ESC, pressed); break;
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

    if (p_Event->type == SDL_EVENT_MOUSE_MOTION)
    {
        UpdateKey(KEY::MOUSE_MOVE, true);
        Context::GetContext()->AppState.MouseRel.x = p_Event->motion.xrel;
        Context::GetContext()->AppState.MouseRel.y = p_Event->motion.yrel;
    }

    else if (p_Event->type == SDL_EVENT_MOUSE_WHEEL)
    {
        UpdateKey(KEY::MOUSE_ROLL, true);
        Context::GetContext()->AppState.ScrollVal.x = p_Event->wheel.x;
        Context::GetContext()->AppState.ScrollVal.y = p_Event->wheel.y;
    }

    else if (p_Event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        UpdateKey(KEY::RESIZE_PROJECT_WINDOW, true);

        ImGuiIO &io = ImGui::GetIO();
        (void)io;

        auto &rndt = Context::GetContext()->RenderData;

        SDL_GetWindowSize(rndt.Window, &rndt.Width, &rndt.Height);
        io.DisplaySize = ImVec2((float)rndt.Width, (float)rndt.Height);
    }

    return SDL_APP_CONTINUE;
}
