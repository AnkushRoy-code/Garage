#include "SimpleImGui.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

#include "Common/Common.h"
#include "Utils/Time.h"

bool SimpleImGui::Init(Context &context)
{
    PROFILE_SCOPE_N("SimpleImGui::Init");
    SDL_SetWindowResizable(context.mWindow, true);

    return true;
}


bool SimpleImGui::Update(Context &context)
{
    PROFILE_SCOPE_N("SimpleImGui::Update");

    ImGui::Begin("Ankush's Garage - ToolBox");
    const auto d = Utils::Time::deltaTime();

    ImGui::Text("%.3f ms/frame (%.1f FPS)", d, 1000.0f / d);

    ImGui::End();

    return true;
}

bool SimpleImGui::Draw(Context &context)
{
    PROFILE_SCOPE_N("SimpleImGui::Draw");
    return true;
}

void SimpleImGui::Quit(Context &context)
{
    PROFILE_SCOPE_N("SimpleImGui::Quit");
    SDL_SetWindowResizable(context.mWindow, false);
    // SDL_SetWindowSize(context.mWindow, 640, 480);
}
