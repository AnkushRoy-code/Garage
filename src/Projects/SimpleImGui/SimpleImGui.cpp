#include "SimpleImGui.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

#include "Common/Common.h"

bool SimpleImGui::Init(Context &context)
{
    PROFILE_SCOPE_N("SimpleImGui::Init");
    SDL_SetWindowResizable(context.mWindow, true);

    return true;
}

void SimpleImGui::handleEvent(SDL_Event &event)
{
    ImGui_ImplSDL3_ProcessEvent(&event);
}

bool SimpleImGui::Update(Context &context)
{
    PROFILE_SCOPE_N("SimpleImGui::Update");
    if (SDL_GetWindowFlags(context.mWindow) & SDL_WINDOW_MINIMIZED)
    {
        SDL_Delay(10);
        return true;
    }

    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    {
        ImGui::Begin("Ankush's Garage");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate,
                    io.Framerate);
        ImGui::End();
    }

    return true;
}

bool SimpleImGui::Draw(Context &context)
{
    PROFILE_SCOPE_N("SimpleImGui::Draw");
    if (SDL_GetWindowFlags(context.mWindow) & SDL_WINDOW_MINIMIZED) { return true; }

    ImGui::Render();
    ImDrawData *draw_data = ImGui::GetDrawData();
    const bool is_minimized =
        (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

    SDL_GPUCommandBuffer *command_buffer =
        SDL_AcquireGPUCommandBuffer(context.mDevice);  // Acquire a GPU command buffer

    SDL_GPUTexture *swapchain_texture;
    SDL_AcquireGPUSwapchainTexture(command_buffer, context.mWindow, &swapchain_texture, nullptr,
                                   nullptr);  // Acquire a swapchain texture

    if (swapchain_texture != nullptr && !is_minimized)
    {
        // This is mandatory: call Imgui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index
        // buffer!
        Imgui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

        // Setup and start a render pass
        SDL_GPUColorTargetInfo target_info = {};
        target_info.texture                = swapchain_texture;
        target_info.clear_color            = SDL_FColor {};
        target_info.load_op                = SDL_GPU_LOADOP_CLEAR;
        target_info.store_op               = SDL_GPU_STOREOP_STORE;
        target_info.mip_level              = 0;
        target_info.layer_or_depth_plane   = 0;
        target_info.cycle                  = false;
        SDL_GPURenderPass *render_pass =
            SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

        // Render ImGui
        ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

        SDL_EndGPURenderPass(render_pass);
    }

    // Submit the command buffer
    SDL_SubmitGPUCommandBuffer(command_buffer);
    return true;
}

void SimpleImGui::Quit(Context &context)
{
    PROFILE_SCOPE_N("SimpleImGui::Quit");
    SDL_SetWindowResizable(context.mWindow, false);
    SDL_SetWindowSize(context.mWindow, 640, 480);
}
