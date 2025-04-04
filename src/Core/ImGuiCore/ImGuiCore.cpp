#include "ImGuiCore.h"

#include "Core/Common/SDL_Exception.h"
#include "Core/Context.h"
#include "Core/Console.h"
#include "Core/Renderer/Renderer.h"
#include "Projects/Common/BaseProject.h"
#include "Utils/Time.h"

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
    SDL_GetWindowSize(gContext.renderData.window, &w, &h);
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

    ImGui_ImplSDL3_InitForSDLGPU(gContext.renderData.window);
    ImGui_ImplSDLGPU3_InitInfo init_info {};
    init_info.Device = gContext.renderData.device;
    init_info.ColorTargetFormat =
        SDL_GetGPUSwapchainTextureFormat(gContext.renderData.device, gContext.renderData.window);
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU3_Init(&init_info);
}

void Core::ImGuiCore::Quit()
{
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

struct ScrollingBuffer
{
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000)
    {
        MaxSize = max_size;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y)
    {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else
        {
            Data[Offset] = ImVec2(x, y);
            Offset       = (Offset + 1) % MaxSize;
        }
    }
    void Erase()
    {
        if (Data.size() > 0)
        {
            Data.shrink(0);
            Offset = 0;
        }
    }
};

void Core::ImGuiCore::Update()
{
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui::NewFrame();

    gContext.appState.mainViewportId = ImGui::DockSpaceOverViewport();

    static bool imguiIniExists = std::filesystem::exists("imgui.ini");
    static bool firstTime      = true;
    static const auto vpID     = gContext.appState.mainViewportId;

    if (firstTime && !imguiIniExists)
    {
        firstTime = false;

        ImGui::DockBuilderRemoveNode(gContext.appState.mainViewportId);
        ImGui::DockBuilderAddNode(gContext.appState.mainViewportId);
        ImGui::DockBuilderSetNodeSize(gContext.appState.mainViewportId,
                                      ImGui::GetMainViewport()->Size);

        auto dockIdRight =
            ImGui::DockBuilderSplitNode(gContext.appState.mainViewportId, ImGuiDir_Right, 0.25,
                                        nullptr, &gContext.appState.mainViewportId);

        auto dockIdBottom =
            ImGui::DockBuilderSplitNode(gContext.appState.mainViewportId, ImGuiDir_Down, 0.3,
                                        nullptr, &gContext.appState.mainViewportId);

        auto dockIdLeft =
            ImGui::DockBuilderSplitNode(gContext.appState.mainViewportId, ImGuiDir_Left, 0.25,
                                        nullptr, &gContext.appState.mainViewportId);

        ImGui::DockBuilderDockWindow("Ankush's Garage - ToolBox", dockIdLeft);
        ImGui::DockBuilderDockWindow("Console", dockIdBottom);
        ImGui::DockBuilderDockWindow("###ProjectUI", dockIdRight);
        ImGui::DockBuilderDockWindow("###TexTitle", gContext.appState.mainViewportId);

        ImGui::DockBuilderFinish(gContext.appState.mainViewportId);
    }

    if (ImGui::Begin("Ankush's Garage - ToolBox"))
    {
        static std::vector<const char *> names;
        static bool firsstTime = true;

        if (firsstTime)
        {
            firsstTime = false;
            names.reserve(Projects.size());
            for (const auto &project: Projects)
            {
                names.push_back(project->getName().c_str());
            }
        }
        ImGui::SeparatorText("Projects");

        // Project selector
        if (ImGui::BeginCombo("Project", names[gContext.appState.projectIndex]))
        {
            for (int i = 0; i < Projects.size(); i++)
            {
                const bool isSelected = (gContext.appState.projectIndex == i);
                if (ImGui::Selectable(names[i], isSelected))
                {
                    if (i != gContext.appState.projectIndex)
                    {
                        Projects[gContext.appState.projectIndex]->Quit();
                        gContext.appState.projectIndex = i;
                        Projects[gContext.appState.projectIndex]->Init();
                    }
                }
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::SeparatorText("Controls");
        // resolution
        static int res = 100;
        ImGui::Text("Scale Resolution");
        if (ImGui::SliderInt("###yetanotherid", &res, 25, 500, "%d%%"))
        {
            gContext.renderData.resolutionScale = res / 100.0f;
            Core::Renderer::ResizeProjectTexture(gContext.appState.ProjectWindowX,
                                                 gContext.appState.ProjectWindowY);
        }

        ImGui::SeparatorText("Data");
        // fps graph
        {
            static ScrollingBuffer fpsBuf;
            const float currentTime = Utils::Time::getTicks() / 1000.0f;
            const float deltaTime   = Utils::Time::deltaTime();
            const float fps         = 1000.0f / deltaTime;
            static float t          = 0;
            t += deltaTime / 1000.0;

            fpsBuf.AddPoint(t, fps);

            static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

            static float history = 10.0f;
            ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

            if (ImPlot::BeginPlot("FPS Plot", ImVec2(-1, 250)))
            {
                ImPlot::SetupAxes("Time (s)", "FPS", flags, 0);
                ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 65);

                ImPlot::PlotLine("FPS", &fpsBuf.Data[0].x, &fpsBuf.Data[0].y, fpsBuf.Data.size(), 0,
                                 fpsBuf.Offset, 2 * sizeof(float));
                ImPlot::EndPlot();
            }
        }
    }
    ImGui::End();

    if (ImGui::Begin("Console"))
    {

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
    }
    ImGui::End();

    // I'll be honest with ya gpt helped me with this one. I was stuck find a way to add new
    // function to my BaseProject without needing to change it in all of the projects...
    // look at ff4cb73 for reference.
    if (Common::BaseProject::hasUI)
    {
        if (auto imgui_project =
                dynamic_cast<Common::ImGuiUI *>(Projects[gContext.appState.projectIndex].get()))
        {
            imgui_project->DrawUI();
        }
    }
}

void Core::ImGuiCore::Draw()
{
    ImGui::Render();
    ImDrawData *drawData = ImGui::GetDrawData();

    SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(gContext.renderData.device);
    if (!commandBuffer) { throw SDL_Exception("AcquireGPUCommandBuffer failed!"); }

    SDL_GPUTexture *swapchainTexture;
    if (!SDL_AcquireGPUSwapchainTexture(commandBuffer, gContext.renderData.window,
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

        static SDL_GPURenderPass *renderPass {};
        renderPass = SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);

        ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer, renderPass);

        SDL_EndGPURenderPass(renderPass);
        SDL_SubmitGPUCommandBuffer(commandBuffer);
    }
}
