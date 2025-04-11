#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>  // only need in main.cpp

#include "Core/Common/pch.h"

#include "Core/Common/Common.h"
#include "Core/Common/SDL_Exception.h"
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
    try
    {
        Core::ConsoleLogBuffer::AddMessage(
            "Welcome to Ankush's Garage\n"
            "This is a Software I made to showcase my Works that I will build over the years."
            "So this is a placeholder for all my stuff! I hope you enjoy what I have made until "
            "now. It might not be much but was HARD");
        g_Context.init();

        Common::ProjectManager::RegisterAllProjects();
        g_Projects = Common::ProjectManager::GetProjects();
        g_Projects[0]->Init();
    }

    catch (const SDL_Exception &e)
    {
        std::cerr << "Error in SDL_AppInit()\n" << e.what() << '\n';
        return SDL_APP_FAILURE;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error something went wrong while SDL_AppInit()\n" << e.what() << '\n';
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
    PROFILE_SCOPE;
    ImGui_ImplSDL3_ProcessEvent(event);
    return g_Context.EventHandler.HandleEvents(event);
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

        g_Projects[g_Context.AppState.ProjectIndex]->Update();
    }
}

/// @brief the update and rendering is done in this callback function
///
/// The Update function is always running at 60fps. If for some reason it is unable to run at 60 and
/// drops below 60, the rendering has to wait. For example if update took 20ms(instead of 16).
/// Rendering will have to wait for 4ms before it starts rendering next frame.
///
/// In the other hand if rendering takes 30ms, Update will not wait. Update will go on at 16ms.
///
/// This is to prevent rendering same thing twice and keep physics based problems at minimum.
SDL_AppResult SDL_AppIterate(void *appstate)
{
    PROFILE_SCOPE;
    try
    {
        g_RenderingDone.store(false);
        std::thread updateProject(updateFunc);  // use jthread when macos supports it
        Tracker::RegisterPoints();

        // no need to draw if window is minimised. But we sure need to update the state.
        if (SDL_GetWindowFlags(g_Context.RenderData.Window) & SDL_WINDOW_MINIMIZED)
        {
            g_RenderingDone.store(true);
            updateProject.join();
            return SDL_APP_CONTINUE;
        }

        static float renderTime = 0.0f;
        Timer riesnt(renderTime);

        Tracker::AddRenderFPSPointQueue(renderTime);

        /// @todo have everything imgui related in a different thread
        // Hours wasted trying that: 0.2 (about 15mins)
        Core::ImGuiCore::Update();

        /// @todo Fix whatever this shit has happened. Can't keep this codeblock inside
        /// Core::ImGuiCore::Update function
        // Hours Wasted: 2
        // This doesn't work with this being inside the ImGuiCore::Update(); function. fsr

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
        // Draw the project to the screen ImGui window
        const auto projectWindowName =
            "Project - " + g_Projects[g_Context.AppState.ProjectIndex]->GetName() + "###TexTitle";

        if (ImGui::Begin(projectWindowName.c_str()))
        {
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

            const SDL_GPUTextureSamplerBinding bind {g_Context.RenderData.ProjectTexture,
                                                     g_Context.RenderData.ProjectSampler};
            const auto size = ImGui::GetWindowSize();

            ImGui::Image((ImTextureID)&bind, {size.x, size.y - 19.0f});
        }
        ImGui::End();
        ImGui::PopStyleVar();

        Core::ImGuiCore::Draw();

        // std::this_thread::sleep_for(std::chrono::milliseconds(100));

        g_RenderingDone.store(true);
        updateProject.join();

        g_Context.EventHandler.EndFrame();

        if (g_Context.AppState.HasToChangeIndex)
        {
            g_Projects[g_Context.AppState.ProjectIndex]->Quit();
            g_Context.AppState.ProjectIndex = g_Context.AppState.ProjectToBeIndex;
            g_Projects[g_Context.AppState.ProjectIndex]->Init();
            g_Context.AppState.HasToChangeIndex = false;
        }
    }

    catch (const SDL_Exception &e)
    {
        std::cerr << "Error in SDL_AppIterate()\n" << e.what() << '\n';
        return SDL_APP_FAILURE;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error something went wrong while SDL_AppIterate()\n" << e.what() << '\n';
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

/// @return false if window is minimised
bool HandleWindowResize()
{
    ImVec2 view = ImGui::GetWindowSize();

    if (view.x != g_Context.AppState.ProjectWindowX || view.y != g_Context.AppState.ProjectWindowY)
    {
        g_Context.EventHandler.UpdateKey(Core::RESIZE_PROJECT_WINDOW, true);

        if (view.x == 0 || view.y == 0) { return false; }  // window is minimised
        g_Context.AppState.ProjectWindowX = view.x;
        g_Context.AppState.ProjectWindowY = view.y;

        Core::Renderer::ResizeProjectTexture(view.x, view.y);
        return true;
    }

    return true;
}
