#include "SDL3/SDL_gpu.h"
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#define SDL_MAIN_USE_CALLBACKS 1
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
        Projects[0]->Init(context);

        Utils::Time::init();
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
    try
    {
        // Don't ask about Pressed_A variable... It's temporary
        bool changeState {false};
        bool Pressed_A {false};  // to detect if A was pressed or D was pressed.
                                 // To properly Quit the running example.

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

        else if (event->type == SDL_EVENT_WINDOW_RESIZED)
        {
            ImGuiIO &io = ImGui::GetIO();
            (void)io;
            int w, h;
            SDL_GetWindowSize(context.mWindow, &w, &h);
            io.DisplaySize = ImVec2((float)w, (float)h);
        }

        // Change Example
        if (changeState)
        {
            if (!Projects.empty() && exampleIndex < Projects.size())
            {
                // Calculate target index
                size_t newIndex = Pressed_A ? std::max(int(0), exampleIndex - 1)
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
        std::cerr << "Error in SDL_AppEvent()\n" << e.what() << '\n';
        return SDL_APP_FAILURE;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error something went wrong while SDL_AppEvent()\n" << e.what() << '\n';
        return SDL_APP_FAILURE;
    }

    catch (...)
    {
        std::cerr << "Some unknown error occured inside SDL_AppEvent() function\n";
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

        Projects[exampleIndex]->Update(context);

        if (SDL_GetWindowFlags(context.mWindow) & SDL_WINDOW_MINIMIZED)
        {
            Utils::Time::capFPS();
            return SDL_APP_CONTINUE;
        }
        // ImGui::DockSpaceOverViewport();  // Incredible performance drop with
        // this one fsr

        if (ImGui::Begin("Ankush's Garage - ToolBox"))
        {
            const auto d = Utils::Time::deltaTime();

            ImGui::Text("%.3f ms/frame (%.1f FPS)", d, 1000.0f / d);
            ImGui::End();
        }

        // Draw the project to the screen ImGui window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
        {
            // clang-format off
            ImGui::Begin("Texture Window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse);
            // clang-format on

            const SDL_GPUTextureSamplerBinding tbind {context.mProjectTexture,
                                                      context.mProjectSampler};
            const auto size = ImGui::GetWindowSize();

            ImGui::Image((ImTextureID)&tbind, {size.x, size.y - 19.0f});
            ImGui::End();
        }

        ImGui::PopStyleVar();

        // Start Drawing
        ImGui::Render();
        ImDrawData *drawData = ImGui::GetDrawData();

        SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(context.mDevice);
        if (!commandBuffer) { throw SDL_Exception("AcquireGPUCommandBuffer failed!"); }

        SDL_GPUTexture *swapchainTexture;
        if (!SDL_AcquireGPUSwapchainTexture(commandBuffer, context.mWindow, &swapchainTexture,
                                            nullptr, nullptr))
        {
            throw SDL_Exception("WaitAndAcquireGPUSwapchainTexture failed!");
        }

        if (swapchainTexture != nullptr)
        {
            // ImGui Pass
            Imgui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer);

            SDL_GPUColorTargetInfo targetInfo = {};
            targetInfo.texture                = swapchainTexture;
            targetInfo.clear_color            = SDL_FColor {0.0, 0.0f, 0.0f, 1.00f};
            targetInfo.load_op                = SDL_GPU_LOADOP_CLEAR;
            targetInfo.store_op               = SDL_GPU_STOREOP_STORE;
            targetInfo.mip_level              = 0;
            targetInfo.layer_or_depth_plane   = 0;
            targetInfo.cycle                  = false;

            context.mRenderPass = SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);

            ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer, context.mRenderPass);

            SDL_EndGPURenderPass(context.mRenderPass);
            SDL_SubmitGPUCommandBuffer(commandBuffer);
            // ImGui Pass end
        }

        SDL_GPUCommandBuffer *commandBufferProjects = SDL_AcquireGPUCommandBuffer(context.mDevice);
        if (!commandBufferProjects)
        {
            throw SDL_Exception("AcquireGPUCommandBuffer(Projects) failed!");
        }

        {
            // Project Pass start
            SDL_GPUColorTargetInfo projectTargetInfo = {};
            projectTargetInfo.texture                = context.mProjectTexture;
            projectTargetInfo.clear_color            = SDL_FColor {0.45f, 0.55f, 0.60f, 1.00f};
            projectTargetInfo.load_op                = SDL_GPU_LOADOP_CLEAR;
            projectTargetInfo.store_op               = SDL_GPU_STOREOP_STORE;
            projectTargetInfo.cycle                  = true;

            context.mProjectPass =
                SDL_BeginGPURenderPass(commandBufferProjects, &projectTargetInfo, 1, nullptr);

            Projects[exampleIndex]->Draw(context);

            SDL_EndGPURenderPass(context.mProjectPass);
            SDL_SubmitGPUCommandBuffer(commandBufferProjects);
            // Project pass end
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

    Utils::Time::capFPS();
    PROFILE_FRAME;
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // Do stuff when needed
}
