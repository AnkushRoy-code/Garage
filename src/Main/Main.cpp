#include "Main.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>  // only need in main.cpp

#include "Core/Common/pch.h"

#include "Core/Common/Common.h"
#include "Core/Context.h"
#include "Core/Event/EventHandler.h"
#include "Core/ImGuiCore/ImGuiCore.h"
#include "Core/ImGuiCore/FPSTracker.h"

#include "Projects/Common/BaseProject.h"

#include "Utils/Timer.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    Garage::InitCoreSystems();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    PROFILE_SCOPE;

    if (Garage::WindowMinimised)
    {
        return SDL_APP_CONTINUE;
    }

    ImGui_ImplSDL3_ProcessEvent(event);
    return Core::Context::GetContext()->EventHandler.HandleEvents(event);
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    PROFILE_SCOPE;
    Garage::InitiateProjectUpdateLoop();

    // no need to draw if window is minimised. But we sure need to update the state.
    if (Garage::WindowMinimised)
    {
        Garage::StopProjectUpdateLoop();
        return SDL_APP_CONTINUE;
    }

    Tracker::RegisterPoints();

    {
        static float renderTime = 0.0f;
        Tracker::AddRenderFPSPointQueue(renderTime);
        Timer temporaryUselessVariable(renderTime);

        Core::ImGuiCore::Update();
        Core::ImGuiCore::Draw();
    }

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Garage::StopProjectUpdateLoop();
    Garage::ChangeProject();

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // Do stuff when needed
}
