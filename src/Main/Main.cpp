#include "Main.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>  // only need in main.cpp

#include "Core/Common/pch.h"

#include "Core/Common/Common.h"
#include "Core/Context.h"
#include "Core/EventHandler.h"
#include "Core/ImGuiCore/ImGuiCore.h"
#include "Core/ImGuiCore/FPSTracker.h"
#include "Core/Renderer/Renderer.h"

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
    ImGui_ImplSDL3_ProcessEvent(event);
    return Core::Context::GetContext()->EventHandler.HandleEvents(event);
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    PROFILE_SCOPE;
    Garage::InitiateProjectUpdateLoop();

    // no need to draw if window is minimised. But we sure need to update the state.

    auto &window = Core::Context::GetContext()->RenderData.Window;

    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
    {
        Garage::StopProjectUpdateLoop();
        return SDL_APP_CONTINUE;
    }

    Tracker::RegisterPoints();

    {
        static float renderTime = 0.0f;
        Tracker::AddRenderFPSPointQueue(renderTime);
        Timer temporaryUselessVariable(renderTime);

        /// @todo Fix whatever this shit has happened. Can't keep the entire Imgui stuff inside
        /// different thread and also the below code block can't be inside any other function let
        /// alone Core::ImGuiCore::Update()
        Core::ImGuiCore::Update();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});

        const auto projName = Common::ProjectManager::GetProjects()
                                  ->at(Core::Context::GetContext()->AppState.ProjectIndex)
                                  ->GetName();

        const auto projectWindowName = "Project - " + projName + "###TexTitle";

        auto &apst = Core::Context::GetContext()->AppState;
        auto ctx   = Core::Context::GetContext();
        if (ImGui::Begin(projectWindowName.c_str()))
        {
            apst.projectWindowFocused = ImGui::IsWindowFocused();
            apst.projectWindowHovered = ImGui::IsWindowHovered();

            if (apst.projectWindowFocused && apst.projectWindowHovered
                && ctx->EventHandler.GetEventPressed(Core::ESC))
            {  // the user wants to get out of focus from the project screen
                ImGui::SetWindowFocus("###ProjectUI");
            }

            if (!Core::ImGuiCore::HandleWindowResize())
            {
                ImGui::End();
                ImGui::PopStyleVar();
                Garage::StopProjectUpdateLoop();
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
