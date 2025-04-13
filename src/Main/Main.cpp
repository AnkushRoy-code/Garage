#include "imgui.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>  // only need in main.cpp

#include "Core/Common/pch.h"

#include "Core/Common/Common.h"
#include "Core/Console.h"
#include "Core/Context.h"
#include "Core/EventHandler.h"
#include "Core/ImGuiCore/ImGuiCore.h"
#include "Core/ImGuiCore/GameFPSTracker.h"
#include "Core/Renderer/Renderer.h"

#include "Projects/Common/BaseProject.h"

#include "Utils/Time.h"
#include "Utils/Timer.h"
#include "Utils/CapFPS.h"

bool HandleWindowResize();
std::atomic<bool> g_RenderingDone;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    Core::ConsoleLogBuffer::AddMessage(
        "Welcome to Ankush's Garage\n"
        "This is a Software I made to showcase my Works that I will build over the years."
        "So this is a placeholder for all my stuff! I hope you enjoy what I have made until "
        "now. It might not be much but was HARD");
    Core::Context::GetContext()->init();
    Common::ProjectManager::RegisterAllProjects();
    g_Projects = Common::ProjectManager::GetProjects();
    g_Projects[0]->Init();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    PROFILE_SCOPE;
    ImGui_ImplSDL3_ProcessEvent(event);
    return Core::Context::GetContext()->EventHandler.HandleEvents(event);
}

void updateFunc()
{
    while (!g_RenderingDone.load())
    {
        Utils::Time::UpdateDeltaTime();
        Utils::CapZone temp(60);

        static float updateTime = 0.0f;

        Timer riesnt(updateTime);
        Tracker::AddUpdateFPSPointQueue(updateTime);

        g_Projects[Core::Context::GetContext()->AppState.ProjectIndex]->Update();
    }
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    PROFILE_SCOPE;

    g_RenderingDone.store(false);
    std::thread updateProject(updateFunc);

    // For FPS Graph
    Tracker::RegisterPoints();
    static float renderTime = 0.0f;
    Timer riesnt(renderTime);
    Tracker::AddRenderFPSPointQueue(renderTime);

    auto ctx = Core::Context::GetContext();

    // no need to draw if window is minimised. But we sure need to update the state.
    if (SDL_GetWindowFlags(ctx->RenderData.Window) & SDL_WINDOW_MINIMIZED)
    {
        g_RenderingDone.store(true);
        updateProject.join();
        return SDL_APP_CONTINUE;
    }

    /// @todo Fix whatever this shit has happened. Can't keep Imgui stuff inside
    /// different thread and the below code block can't be inside Core::ImGuiCore::Update
    Core::ImGuiCore::Update();

    auto &apst = Core::Context::GetContext()->AppState;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
    const auto projectWindowName =
        "Project - " + g_Projects[apst.ProjectIndex]->GetName() + "###TexTitle";

    if (ImGui::Begin(projectWindowName.c_str()))
    {
        apst.projectWindowFocused = ImGui::IsWindowFocused();
        apst.projectWindowHovered = ImGui::IsWindowHovered();

        if (apst.projectWindowFocused && apst.projectWindowHovered
            && ctx->EventHandler.GetEventPressed(Core::ESC))
        {  // the user wants to get out of focus from the project screen
            ImGui::SetWindowFocus("###ProjectUI");
        }
        if (!HandleWindowResize())
        {
            ImGui::End();
            ImGui::PopStyleVar();
            g_RenderingDone.store(true);
            updateProject.join();
            return SDL_APP_CONTINUE;
        }

        // Note to self: Moving this function down causes artifacts when resizing
        Core::Renderer::DrawProjectToTexture();

        const SDL_GPUTextureSamplerBinding bind {ctx->RenderData.ProjectTexture,
                                                 ctx->RenderData.ProjectSampler};

        // Draw the project to the screen ImGui window
        const auto size = ImGui::GetWindowSize();
        ImGui::Image((ImTextureID)&bind, {size.x, size.y - 19.0f});
    }
    ImGui::End();
    ImGui::PopStyleVar();

    Core::ImGuiCore::Draw();

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    g_RenderingDone.store(true);
    updateProject.join();

    ctx->EventHandler.EndFrame();

    if (apst.HasToChangeIndex)
    {
        g_Projects[apst.ProjectIndex]->Quit();
        apst.ProjectIndex = apst.ProjectToBeIndex;
        g_Projects[apst.ProjectIndex]->Init();
        apst.HasToChangeIndex = false;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // Do stuff when needed
}

/// @return false if window is minimised
bool HandleWindowResize()
{
    ImVec2 view = ImGui::GetWindowSize();
    auto ctx    = Core::Context::GetContext();

    if (view.x != ctx->AppState.ProjectWindowSize.x || view.y != ctx->AppState.ProjectWindowSize.y)
    {
        ctx->EventHandler.UpdateKey(Core::RESIZE_PROJECT_WINDOW, true);

        if (view.x == 0 || view.y == 0) { return false; }  // window is minimised
        ctx->AppState.ProjectWindowSize.x = view.x;
        ctx->AppState.ProjectWindowSize.y = view.y;

        Core::Renderer::ResizeProjectTexture(view.x, view.y);
        return true;
    }

    return true;
}
