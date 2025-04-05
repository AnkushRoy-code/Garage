#include "Utils/CapFPS.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>  // only need in main.cpp
#include "Core/Common/pch.h"

#include "Core/Common/Common.h"
#include "Core/Common/SDL_Exception.h"
#include "Core/Console.h"
#include "Core/Context.h"
#include "Core/EventHandler.h"
#include "Core/ImGuiCore/ImGuiCore.h"
#include "Core/Renderer/Renderer.h"

#include "Projects/Common/BaseProject.h"

#include "Utils/Time.h"

bool HandleWindowResize();
std::atomic<bool> renderingDone;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    try
    {
        Core::ConsoleLogBuffer::addMessage(
            "Welcome to Ankush's Garage\n"
            "This is a Software I made to showcase my Works that I will build over the years."
            "So this is a placeholder for all my stuff! I hope you enjoy what I have made until "
            "now. It might not be much but was HARD");
        gContext.init();

        Common::ProjectManager::registerAllProjects();
        Projects = Common::ProjectManager::getProjects();
        Projects[0]->Init();
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
    return Core::EventHandler::handleEvents(event, gContext.inputHandler);
}

void updateFunc()
{
    while (!renderingDone.load())
    {
        Utils::CapZone temp(60);
        Projects[gContext.appState.projectIndex]->Update();
    }
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    PROFILE_SCOPE;
    try
    {
        Utils::Time::updateDeltaTime();

        renderingDone.store(false);
        std::jthread updateProject(updateFunc);

        // no need to draw if window is minimised. But we sure need to update the state.
        if (SDL_GetWindowFlags(gContext.renderData.window) & SDL_WINDOW_MINIMIZED)
        {
            Utils::Time::capFPS();
            return SDL_APP_CONTINUE;
        }

        Core::ImGuiCore::Update();

        /// @todo Fix whatever this shit has happened. Can't keep this codeblock inside
        /// Core::ImGuiCore::Update function
        // Hours Wasted: 2
        // This doesn't work with this being inside the ImGuiCore::Update(); function. fsr
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
        {
            // Draw the project to the screen ImGui window
            const auto projectWindowName =
                "Project - " + Projects[gContext.appState.projectIndex]->getName() + "###TexTitle";
            ImGui::Begin(projectWindowName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse);

            if (!HandleWindowResize())
            {
                ImGui::End();
                ImGui::PopStyleVar();
                return SDL_APP_CONTINUE;
            }

            const SDL_GPUTextureSamplerBinding bind {gContext.renderData.projectTexture,
                                                     gContext.renderData.projectSampler};
            const auto size = ImGui::GetWindowSize();

            ImGui::Image((ImTextureID)&bind, {size.x, size.y - 19.0f});
            ImGui::End();
        }

        ImGui::PopStyleVar();

        Core::ImGuiCore::Draw();

        Core::Renderer::DrawProjectToTexture();

        renderingDone.store(true);
        updateProject.join();

        gContext.inputHandler.endFrame();
        Utils::Time::capFPS();
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

    if (view.x != gContext.appState.ProjectWindowX || view.y != gContext.appState.ProjectWindowY)
    {
        gContext.inputHandler.updateKey(Core::RESIZE_PROJECT_WINDOW, true);

        if (view.x == 0 || view.y == 0) { return false; }  // window is minimised
        gContext.appState.ProjectWindowX = view.x;
        gContext.appState.ProjectWindowY = view.y;

        Core::Renderer::ResizeProjectTexture(view.x, view.y);
        return true;
    }

    return true;
}
