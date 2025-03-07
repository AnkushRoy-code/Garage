#include "ImGui.h"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlgpu3.h"

bool SimpleImGui::Init(Context &context)
{
    SDL_SetWindowResizable(context.mWindow, true);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLGPU(context.mWindow);
    ImGui_ImplSDLGPU3_InitInfo init_info {};
    init_info.Device = context.mDevice;
    init_info.ColorTargetFormat =
        SDL_GetGPUSwapchainTextureFormat(context.mDevice, context.mWindow);
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU3_Init(&init_info);

    return true;
}

void SimpleImGui::handleEvent(SDL_Event &event)
{
    ImGui_ImplSDL3_ProcessEvent(&event);
}

bool show_demo_window    = true;
bool show_another_window = false;
ImVec4 clear_color       = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

bool SimpleImGui::Update(Context &context)
{
    if (SDL_GetWindowFlags(context.mWindow) & SDL_WINDOW_MINIMIZED)
    {
        SDL_Delay(10);
        return true;
    }

    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can
    // browse its code to learn more about Dear ImGui!).
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named
    // window.
    {
        static float f     = 0.0f;
        static int counter = 0;

        ImGui::Begin(
            "Hello, world!");  // Create a window called "Hello, world!" and append into it.

        ImGui::Text(
            "This is some useful text.");  // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window",
                        &show_demo_window);  // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f,
                           1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit4("clear color",
                          (float *)&clear_color);  // Edit 3 floats representing a color

        if (ImGui::Button("Button"))  // Buttons return true when clicked (most widgets return true
                                      // when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate,
                    io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin(
            "Another Window",
            &show_another_window);  // Pass a pointer to our bool variable (the window will have a
                                    // closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me")) show_another_window = false;
        ImGui::End();
    }

    return true;
}

bool SimpleImGui::Draw(Context &context)
{
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
        target_info.clear_color =
            SDL_FColor {clear_color.x, clear_color.y, clear_color.z, clear_color.w};
        target_info.load_op              = SDL_GPU_LOADOP_CLEAR;
        target_info.store_op             = SDL_GPU_STOREOP_STORE;
        target_info.mip_level            = 0;
        target_info.layer_or_depth_plane = 0;
        target_info.cycle                = false;
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
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
    ImGui::DestroyContext();
    SDL_SetWindowResizable(context.mWindow, false);
}
