#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

#include "Common/BaseProject.h"
#include "Common/Context.h"
#include "Common/Common.h"
#include "Utils/Time.h"
#include "Common/SDL_Exception.h"

Context context;
std::vector<std::unique_ptr<BaseProject>> Projects {};
int exampleIndex {};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    try
    {
        context.init();
        ProjectManager::registerAllProjects();

        Projects = ProjectManager::getProjects();

        // std::cout << "The Example name is: " << Examples[0].Name <<
        // std::endl; Manually init the first example
        Projects[0]->Init(context);  // Error handling can wait

        Utils::Time::init();
    }

    catch (const SDL_Exception &e)
    {
        std::cerr << "Error in SDL_AppInit()\n" << e.what();
        return SDL_APP_FAILURE;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error something went wrong while SDL_AppInit()\n"
                  << e.what();
        return SDL_APP_FAILURE;
    }

    catch (...)
    {
        std::cerr
            << "Some unknown error occured inside SDL_AppInit() function\n";
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    PROFILE_SCOPE;
    ImGui_ImplSDL3_ProcessEvent(event);
    try
    {
        bool changeState {false};
        bool Pressed_A {false};  // to detect if A was pressed or D was pressed.
                                 // To properly Quit the running example.

        Projects[exampleIndex]->handleEvent(*event);
        if (event->type == SDL_EVENT_QUIT)
        {
            if (exampleIndex != -1) { Projects[exampleIndex]->Quit(context); }
            return SDL_APP_SUCCESS;
        }

        else if (event->type == SDL_EVENT_KEY_DOWN)
        {
            switch (event->key.scancode)
            {
                case SDL_SCANCODE_D:
                {
                    changeState = true;
                    Pressed_A   = false;
                    break;
                };

                case SDL_SCANCODE_A:
                {
                    changeState = true;
                    Pressed_A   = true;
                    break;
                };

                case SDL_SCANCODE_LEFT: context.left = true; break;
                case SDL_SCANCODE_RIGHT: context.right = true; break;
                case SDL_SCANCODE_DOWN: context.down = true; break;
                case SDL_SCANCODE_UP: context.up = true; break;

                default: break;
            }
        }

        else if (event->type == SDL_EVENT_KEY_UP)
        {
            switch (event->key.scancode)
            {
                case SDL_SCANCODE_LEFT: context.left = false; break;
                case SDL_SCANCODE_RIGHT: context.right = false; break;
                case SDL_SCANCODE_DOWN: context.down = false; break;
                case SDL_SCANCODE_UP: context.up = false; break;
                default: break;
            }
        }

        // Change Example
        if (changeState)
        {
            if (!Projects.empty() && exampleIndex < Projects.size())
            {
                // Calculate target index
                size_t newIndex =
                    Pressed_A
                        ? std::max(int(0), exampleIndex - 1)
                        : std::min(exampleIndex + 1, int(Projects.size()) - 1);
                // Perform transition
                if (newIndex != exampleIndex)
                {
                    Projects[exampleIndex]->Quit(context);
                    exampleIndex = newIndex;
                    Projects[exampleIndex]->Init(context);
                }
            }
        }
    }

    catch (const SDL_Exception &e)
    {
        std::cerr << "Error in SDL_AppEvent()\n" << e.what();
        return SDL_APP_FAILURE;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error something went wrong while SDL_AppEvent()\n"
                  << e.what();
        return SDL_APP_FAILURE;
    }

    catch (...)
    {
        std::cerr
            << "Some unknown error occured inside SDL_AppEvent() function\n";
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    try
    {
        Utils::Time::updateDeltaTime();
        PROFILE_SCOPE;

        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDLGPU3_NewFrame();
        ImGui::NewFrame();

        // ImGui::DockSpaceOverViewport();

        {
            ImGui::Begin("Ankush's Garage - ToolBox");
            const auto d = Utils::Time::deltaTime();

            ImGui::Text("%.3f ms/frame (%.1f FPS)", d, 1000.0f / d);

            ImGui::End();
        }

        Projects[exampleIndex]->Update(context);

        // Start Drawing

        ImGui::Render();
        ImDrawData *draw_data = ImGui::GetDrawData();

        if (SDL_GetWindowFlags(context.mWindow) & SDL_WINDOW_MINIMIZED)
        {
            return SDL_APP_CONTINUE;
        }

        SDL_GPUCommandBuffer *commandBuffer =
            SDL_AcquireGPUCommandBuffer(context.mDevice);
        if (!commandBuffer)
        {
            throw SDL_Exception("AcquireGPUCommandBuffer failed!");
        }

        SDL_GPUTexture *swapchainTexture;
        if (!SDL_AcquireGPUSwapchainTexture(commandBuffer, context.mWindow,
                                            &swapchainTexture, nullptr,
                                            nullptr))
        {
            throw SDL_Exception("WaitAndAcquireGPUSwapchainTexture failed!");
        }

        if (swapchainTexture != nullptr)
        {
            Imgui_ImplSDLGPU3_PrepareDrawData(draw_data, commandBuffer);

            SDL_GPUColorTargetInfo target_info = {};
            target_info.texture                = swapchainTexture;
            target_info.clear_color = SDL_FColor {0.45f, 0.55f, 0.60f, 1.00f};
            target_info.load_op     = SDL_GPU_LOADOP_CLEAR;
            target_info.store_op    = SDL_GPU_STOREOP_STORE;
            target_info.mip_level   = 0;
            target_info.layer_or_depth_plane = 0;
            target_info.cycle                = false;

            context.mRenderPass =
                SDL_BeginGPURenderPass(commandBuffer, &target_info, 1, nullptr);

            ImGui_ImplSDLGPU3_RenderDrawData(draw_data, commandBuffer,
                                             context.mRenderPass);

            Projects[exampleIndex]->Draw(context);

            SDL_EndGPURenderPass(context.mRenderPass);
        }

        SDL_SubmitGPUCommandBuffer(commandBuffer);
    }

    catch (const SDL_Exception &e)
    {
        std::cerr << "Error in SDL_AppIterate()\n" << e.what();
        return SDL_APP_FAILURE;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error something went wrong while SDL_AppIterate()\n"
                  << e.what();
        return SDL_APP_FAILURE;
    }

    catch (...)
    {
        std::cerr
            << "Some unknown error occured inside SDL_AppIterate() function\n";
        return SDL_APP_FAILURE;
    }

    Utils::Time::capFPS();
    PROFILE_FRAME;
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // Do stuff when needed
}
