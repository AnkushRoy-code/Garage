#include "Core/Renderer/Renderer.h"
#include <iostream>
#include <memory>
#include <vector>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>

#include "Core/Context.h"
#include "Core/Console.h"
#include "Core/EventHandler.h"
#include "Core/ImGuiCore/ImGuiCore.h"
#include "Common/BaseProject.h"
#include "Common/Common.h"
#include "Common/SDL_Exception.h"
#include "Utils/Time.h"

bool HandleWindowResize();

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    try
    {
        // clang-format off
        Core::ConsoleLogBuffer::addMessage(
            "Welcome to Ankush's Garage\n"
            "Press 'a' or 'd' to change between projects\n\n"
            "Help: The keyboard layout doesn't matter, use the physical "
            "keys in your keyboard to change the examples"
        );
        // clang-format on
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

SDL_AppResult SDL_AppIterate(void *appstate)
{
    try
    {
        PROFILE_SCOPE;
        Utils::Time::updateDeltaTime();

        Projects[gContext.appState.projectIndex]->Update();

        // no need to draw if window is minimised. But sure need to update the state. If needed
        // pause it. Idk why I keep updating like it even matters. But gud design if it ever matters
        if (SDL_GetWindowFlags(gContext.renderData.window) & SDL_WINDOW_MINIMIZED)
        {
            Utils::Time::capFPS();
            Core::ConsoleLogBuffer::addMessage("The thing is minimised...");
            return SDL_APP_CONTINUE;
        }

        Core::ImGuiCore::Update();

        // TODO: Fix whatever this shit has happened.
        // Hours Wasted: 1
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

            Core::Renderer::DrawProjectToTexture();

            const SDL_GPUTextureSamplerBinding bind {gContext.renderData.projectTexture,
                                                     gContext.renderData.projectSampler};
            const auto size = ImGui::GetWindowSize();

            ImGui::Image((ImTextureID)&bind, {size.x, size.y - 19.0f});
            ImGui::End();
        }
        ImGui::PopStyleVar();

        Core::ImGuiCore::Draw();
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

    gContext.inputHandler.endFrame();
    Utils::Time::capFPS();

    PROFILE_FRAME;
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // Do stuff when needed
}

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
