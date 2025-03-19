#include "ImGuiCore.h"

#include "Common/SDL_Exception.h"
#include "Core/Console.h"
#include "Core/Context.h"
#include "Common/BaseProject.h"
#include "SDL3/SDL_gpu.h"
#include "Utils/Time.h"
#include <backends/imgui_impl_sdl3.h>

#include <filesystem>

#include <backends/imgui_impl_sdlgpu3.h>
#include <imgui_internal.h>

void ImGuiCore::Init()
{
    // ImGui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    int w, h;
    SDL_GetWindowSize(gContext.mWindow, &w, &h);
    io.DisplaySize = ImVec2((float)w, (float)h);

    ImGui::StyleColorsDark();

    ImGuiStyle &style        = ImGui::GetStyle();
    style.FrameRounding      = 2.0f;
    style.GrabRounding       = 2.0f;
    style.WindowBorderSize   = 0.0f;
    style.WindowTitleAlign   = ImVec2(0.5f, 0.5f);
    style.WindowRounding     = 4.0f;
    style.SeparatorTextAlign = ImVec2(0.5f, 0.5f);

    ImVec4 *colors                        = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                 = ImVec4(0.80f, 0.84f, 0.96f, 1.00f);
    colors[ImGuiCol_TextDisabled]         = ImVec4(0.42f, 0.44f, 0.53f, 1.00f);
    colors[ImGuiCol_WindowBg]             = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_PopupBg]              = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_Border]               = ImVec4(0.71f, 0.75f, 1.00f, 0.31f);
    colors[ImGuiCol_FrameBg]              = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.35f, 0.36f, 0.44f, 0.73f);
    colors[ImGuiCol_FrameBgActive]        = ImVec4(0.50f, 0.52f, 0.61f, 0.64f);
    colors[ImGuiCol_TitleBg]              = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_TitleBgActive]        = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.00f, 0.00f, 0.00f, 0.75f);
    colors[ImGuiCol_MenuBarBg]            = ImVec4(0.27f, 0.28f, 0.35f, 0.66f);
    colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.07f, 0.07f, 0.11f, 0.48f);
    colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.09f, 0.57f, 0.60f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.02f, 0.65f, 0.90f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.13f, 0.62f, 0.71f, 1.00f);
    colors[ImGuiCol_CheckMark]            = ImVec4(0.54f, 0.71f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]           = ImVec4(0.02f, 0.65f, 0.90f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.13f, 0.62f, 0.71f, 1.00f);
    colors[ImGuiCol_Button]               = ImVec4(0.12f, 0.40f, 0.96f, 0.37f);
    colors[ImGuiCol_ButtonHovered]        = ImVec4(0.12f, 0.40f, 0.96f, 0.61f);
    colors[ImGuiCol_ButtonActive]         = ImVec4(0.12f, 0.40f, 0.96f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.10f, 0.40f, 0.75f, 0.94f);
    colors[ImGuiCol_SeparatorActive]      = ImVec4(0.03f, 0.30f, 0.61f, 1.00f);
    colors[ImGuiCol_ResizeGrip]           = ImVec4(0.15f, 0.53f, 0.83f, 0.57f);
    colors[ImGuiCol_DockingEmptyBg]       = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_PlotLines]            = ImVec4(0.98f, 0.89f, 0.69f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.87f, 0.12f, 0.33f, 1.00f);
    colors[ImGuiCol_PlotHistogram]        = ImVec4(0.65f, 0.89f, 0.63f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.38f, 0.95f, 0.33f, 1.00f);
    colors[ImGuiCol_TabHovered]           = ImVec4(0.50f, 0.52f, 0.61f, 1.00f);
    colors[ImGuiCol_Tab]                  = ImVec4(0.35f, 0.36f, 0.44f, 1.00f);
    colors[ImGuiCol_TabSelected]          = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline]  = ImVec4(0.29f, 0.30f, 0.41f, 0.80f);
    colors[ImGuiCol_TabDimmedSelected]    = ImVec4(0.42f, 0.44f, 0.53f, 0.62f);

    ImGui_ImplSDL3_InitForSDLGPU(gContext.mWindow);
    ImGui_ImplSDLGPU3_InitInfo init_info {};
    init_info.Device = gContext.mDevice;
    init_info.ColorTargetFormat =
        SDL_GetGPUSwapchainTextureFormat(gContext.mDevice, gContext.mWindow);
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU3_Init(&init_info);
}

void ImGuiCore::Quit()
{
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiCore::Update()
{
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui::NewFrame();

    gContext.mainViewportId = ImGui::DockSpaceOverViewport();

    static bool imguiIniExists = std::filesystem::exists("imgui.ini");
    static bool firstTime      = true;

    if (firstTime && !imguiIniExists)
    {
        firstTime = false;

        ImGui::DockBuilderRemoveNode(gContext.mainViewportId);
        ImGui::DockBuilderAddNode(gContext.mainViewportId);
        ImGui::DockBuilderSetNodeSize(gContext.mainViewportId, ImGui::GetMainViewport()->Size);

        auto dockIdLeft = ImGui::DockBuilderSplitNode(gContext.mainViewportId, ImGuiDir_Left, 0.3,
                                                      nullptr, &gContext.mainViewportId);

        auto dockIdBottom = ImGui::DockBuilderSplitNode(gContext.mainViewportId, ImGuiDir_Down, 0.2,
                                                        nullptr, &gContext.mainViewportId);

        auto dockIdRight = ImGui::DockBuilderSplitNode(gContext.mainViewportId, ImGuiDir_Right, 0.4,
                                                       nullptr, &gContext.mainViewportId);

        const char *projectWindowName = "###TexTitle";

        ImGui::DockBuilderDockWindow("Ankush's Garage - ToolBox", dockIdLeft);
        ImGui::DockBuilderDockWindow("Console", dockIdBottom);
        ImGui::DockBuilderDockWindow("Control Panel", dockIdRight);
        ImGui::DockBuilderDockWindow(projectWindowName, gContext.mainViewportId);

        ImGui::DockBuilderFinish(gContext.mainViewportId);
    }

    {
        ImGui::Begin("Ankush's Garage - ToolBox");
        const auto d = Utils::Time::deltaTime();

        ImGui::Text("%.3f ms/frame (%.1f FPS)", d, 1000.0f / d);
        ImGui::End();
    }

    {
        ImGui::Begin("Console");

        const auto cl = ConsoleLogBuffer::ConsoleLogs;

        if (ImGui::BeginTable("ConsoleLogWindowTable", 2,
                              ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_BordersInner))
        {
            ImGui::TableSetupColumn("Console Message", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 70);
            for (int i = 0; i < cl.size(); ++i)
            {
                if (i % 2 == 0)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                else
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

                const auto &lg = cl[i];
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextWrapped("%s", lg.message.c_str());
                ImGui::TableNextColumn();
                ImGui::TextWrapped("[%s]", lg.timestamp.c_str());

                ImGui::PopStyleColor();
            }
            ImGui::EndTable();
        }
        ImGui::End();
    }

    // I'll be honest with ya gpt helped me with this one. I was stuck find a way to add new
    // function to my BaseProject without needing to change it in all of the projects...
    // look at ff4cb73 for reference.
    if (BaseProject::hasUI)
    {
        if (auto imgui_project = dynamic_cast<ImGuiUI *>(Projects[gContext.projectIndex].get()))
        {
            imgui_project->DrawUI();
        }
    }
}

void ImGuiCore::Draw()
{
    ImGui::Render();
    ImDrawData *drawData = ImGui::GetDrawData();

    SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(gContext.mDevice);
    if (!commandBuffer) { throw SDL_Exception("AcquireGPUCommandBuffer failed!"); }

    SDL_GPUTexture *swapchainTexture;
    if (!SDL_AcquireGPUSwapchainTexture(commandBuffer, gContext.mWindow, &swapchainTexture, nullptr,
                                        nullptr))
    {
        throw SDL_Exception("WaitAndAcquireGPUSwapchainTexture failed!");
    }

    if (swapchainTexture != nullptr)
    {
        Imgui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer);

        const SDL_GPUColorTargetInfo targetInfo {
            .texture              = swapchainTexture,
            .mip_level            = 0,
            .layer_or_depth_plane = 0,
            .clear_color          = SDL_FColor {0.0, 0.0f, 0.0f, 1.00f},
            .load_op              = SDL_GPU_LOADOP_CLEAR,
            .store_op             = SDL_GPU_STOREOP_STORE,
            .cycle                = false,

        };

        static SDL_GPURenderPass *renderPass {};
        renderPass = SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);

        ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer, renderPass);

        SDL_EndGPURenderPass(renderPass);
        SDL_SubmitGPUCommandBuffer(commandBuffer);
    }
}
