#include "ImGuiCore.h"

#include "Core/Common/SDL_Exception.h"
#include "Core/Context.h"
#include "Core/Console.h"
#include "Core/ImGuiCore/ScrollingBuffer.h"
#include "Core/ImGuiCore/GameFPSTracker.h"
#include "Core/Renderer/Renderer.h"
#include "Projects/Common/BaseProject.h"

#include "Core/Common/pch.h"

#include <imgui_internal.h>
#include <implot.h>

void Core::ImGuiCore::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    int w, h;
    SDL_GetWindowSize(g_Context.RenderData.Window, &w, &h);
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

    ImGui_ImplSDL3_InitForSDLGPU(g_Context.RenderData.Window);
    ImGui_ImplSDLGPU3_InitInfo initInfo {};
    initInfo.Device = g_Context.RenderData.Device;
    initInfo.ColorTargetFormat =
        SDL_GetGPUSwapchainTextureFormat(g_Context.RenderData.Device, g_Context.RenderData.Window);
    initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU3_Init(&initInfo);
}

void Core::ImGuiCore::Quit()
{
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void Core::ImGuiCore::Update()
{
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui::NewFrame();

    g_Context.AppState.MainViewportId = ImGui::DockSpaceOverViewport();

    static bool x_ImguiIniExists = std::filesystem::exists("imgui.ini");
    static bool x_FirstTime      = true;
    static const auto k_vpID     = g_Context.AppState.MainViewportId;

    if (x_FirstTime && !x_ImguiIniExists)
    {
        x_FirstTime = false;

        ImGui::DockBuilderRemoveNode(g_Context.AppState.MainViewportId);
        ImGui::DockBuilderAddNode(g_Context.AppState.MainViewportId);
        ImGui::DockBuilderSetNodeSize(g_Context.AppState.MainViewportId,
                                      ImGui::GetMainViewport()->Size);

        auto dockIdRight =
            ImGui::DockBuilderSplitNode(g_Context.AppState.MainViewportId, ImGuiDir_Right, 0.25,
                                        nullptr, &g_Context.AppState.MainViewportId);

        auto dockIdBottom =
            ImGui::DockBuilderSplitNode(g_Context.AppState.MainViewportId, ImGuiDir_Down, 0.3,
                                        nullptr, &g_Context.AppState.MainViewportId);

        auto dockIdLeft =
            ImGui::DockBuilderSplitNode(g_Context.AppState.MainViewportId, ImGuiDir_Left, 0.25,
                                        nullptr, &g_Context.AppState.MainViewportId);

        ImGui::DockBuilderDockWindow("Ankush's Garage - ToolBox", dockIdLeft);
        ImGui::DockBuilderDockWindow("Console", dockIdBottom);
        ImGui::DockBuilderDockWindow("###ProjectUI", dockIdRight);
        ImGui::DockBuilderDockWindow("###TexTitle", g_Context.AppState.MainViewportId);

        ImGui::DockBuilderFinish(g_Context.AppState.MainViewportId);
    }

    if (ImGui::Begin("Ankush's Garage - ToolBox"))
    {
        static std::vector<const char *> x_Names;
        static bool x_FirsstTime = true;

        if (x_FirsstTime)
        {
            x_FirsstTime = false;
            x_Names.reserve(g_Projects.size());
            for (const auto &project: g_Projects)
            {
                x_Names.push_back(project->GetName().c_str());
            }
        }
        ImGui::SeparatorText("Projects");

        // Project selector
        if (ImGui::BeginCombo("Project", x_Names[g_Context.AppState.ProjectIndex]))
        {
            for (int i = 0; i < g_Projects.size(); i++)
            {
                const bool isSelected = (g_Context.AppState.ProjectIndex == i);
                if (ImGui::Selectable(x_Names[i], isSelected))
                {
                    if (i != g_Context.AppState.ProjectIndex)
                    {
                        g_Context.AppState.HasToChangeIndex = true;
                        g_Context.AppState.ProjectToBeIndex = i;
                    }
                }
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::SeparatorText("Controls");
        // resolution
        static int x_Res = 100;
        ImGui::Text("Scale Resolution");
        if (ImGui::SliderInt("###yetanotherid", &x_Res, 25, 500, "%d%%"))
        {
            g_Context.RenderData.ResolutionScale = x_Res / 100.0f;
            Core::Renderer::ResizeProjectTexture(g_Context.AppState.ProjectWindowX,
                                                 g_Context.AppState.ProjectWindowY);
        }

        ImGui::SeparatorText("Data");
        // fps graph
        {
            const float t = SDL_GetTicks() / 1000.0f;

            static const ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels
                                                 | ImPlotAxisFlags_NoGridLines
                                                 | ImPlotAxisFlags_NoTickMarks;

            static float x_History = 10.0f;
            ImGui::SliderFloat("History", &x_History, 1, 30, "%.1f s");

            if (ImPlot::BeginPlot("FPS Plot", ImVec2(-1, 250),
                                  ImPlotFlags_NoBoxSelect | ImPlotFlags_Crosshairs))
            {
                ImPlot::SetupLegend(ImPlotLocation_SouthEast);
                ImPlot::SetupMouseText(ImPlotLocation_SouthWest);
                ImPlot::SetupAxes("Time (s)", "FPS", flags, 0);
                ImPlot::SetupAxisLimits(ImAxis_X1, t - x_History, t, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 65);
                ImPlot::SetupFinish();

                ImPlot::PlotLine("Update", &Tracker::s_UpdateFPSBuffer.Data[0].x,
                                 &Tracker::s_UpdateFPSBuffer.Data[0].y,
                                 Tracker::s_UpdateFPSBuffer.Data.size(), 0,
                                 Tracker::s_UpdateFPSBuffer.Offset, 2 * sizeof(float));

                ImPlot::PlotLine("RealUpdate", &Tracker::s_RealUpdateFPSBuffer.Data[0].x,
                                 &Tracker::s_RealUpdateFPSBuffer.Data[0].y,
                                 Tracker::s_RealUpdateFPSBuffer.Data.size(), 0,
                                 Tracker::s_RealUpdateFPSBuffer.Offset, 2 * sizeof(float));

                ImPlot::PlotLine("Render", &Tracker::s_RenderFPSBuffer.Data[0].x,
                                 &Tracker::s_RenderFPSBuffer.Data[0].y,
                                 Tracker::s_RenderFPSBuffer.Data.size(), 0,
                                 Tracker::s_RenderFPSBuffer.Offset, 2 * sizeof(float));

                ImPlot::EndPlot();
            }
        }
    }
    ImGui::End();

    if (ImGui::Begin("Console"))
    {
        const auto cl = ConsoleLogBuffer::GetMessages();

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
    }
    ImGui::End();

    // I'll be honest with ya gpt helped me with this one. I was stuck find a way to add new
    // function to my BaseProject without needing to change it in all of the projects...
    // look at ff4cb73 for reference.
    if (Common::BaseProject::hasUI)
    {
        if (auto imguiProject =
                dynamic_cast<Common::ImGuiUI *>(g_Projects[g_Context.AppState.ProjectIndex].get()))
        {
            imguiProject->DrawUI();
        }
    }
}

void Core::ImGuiCore::Draw()
{
    ImGui::Render();
    ImDrawData *drawData = ImGui::GetDrawData();

    SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(g_Context.RenderData.Device);
    if (!commandBuffer) { throw SDL_Exception("AcquireGPUCommandBuffer failed!"); }

    SDL_GPUTexture *swapchainTexture;
    if (!SDL_AcquireGPUSwapchainTexture(commandBuffer, g_Context.RenderData.Window,
                                        &swapchainTexture, nullptr, nullptr))
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

        static SDL_GPURenderPass *x_RenderPass {};
        x_RenderPass = SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);

        ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer, x_RenderPass);

        SDL_EndGPURenderPass(x_RenderPass);
        SDL_SubmitGPUCommandBuffer(commandBuffer);
    }
}
