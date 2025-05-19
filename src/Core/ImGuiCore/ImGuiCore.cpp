#include "Core/ImGuiCore/ImGuiCore.h"

#include "Core/Context.h"
#include "Core/Console.h"
#include "Core/ImGuiCore/DataPlotter.h"
#include "Core/Renderer/Renderer.h"
#include "Main/Main.h"
#include "Projects/Common/BaseProject.h"

#include "Core/Common/pch.h"

#include <imgui_internal.h>
#include <implot.h>

namespace Core
{

SDL_GPUTextureSamplerBinding ImGuiCore::bind;

void ImGuiCore::Init()
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
    SDL_GetWindowSize(Core::Context::GetContext()->RenderData.Window, &w, &h);
    io.DisplaySize = ImVec2((float)w, (float)h);

    InitImGuiStyle();

    auto &rndt = Context::GetContext()->RenderData;
    ImGui_ImplSDL3_InitForSDLGPU(rndt.Window);
    ImGui_ImplSDLGPU3_InitInfo initInfo {};
    initInfo.Device            = rndt.Device;
    initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(rndt.Device, rndt.Window);
    initInfo.MSAASamples       = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU3_Init(&initInfo);
}

void ImGuiCore::Quit()
{
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void ImGuiCore::Update()
{
    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui::NewFrame();

    auto &apst = Context::GetContext()->AppState;

    SetImGuiWindowProportions();
    ShowToolBox();
    ShowConsole();
    ShowProjectWindow();
    ShowProjectRendered();
}

void ImGuiCore::Draw()
{
    ImGui::Render();
    ImDrawData *drawData = ImGui::GetDrawData();

    SDL_GPUCommandBuffer *commandBuffer =
        SDL_AcquireGPUCommandBuffer(Core::Context::GetContext()->RenderData.Device);
    assert(commandBuffer);

    SDL_GPUTexture *swapchainTexture;

    SDL_AcquireGPUSwapchainTexture(commandBuffer, Context::GetContext()->RenderData.Window,
                                   &swapchainTexture, nullptr, nullptr);

    assert(swapchainTexture);

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

void ImGuiCore::InitImGuiStyle()
{
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
}

void ImGuiCore::SetImGuiWindowProportions()
{
    auto &apst          = Context::GetContext()->AppState;
    apst.MainViewportId = ImGui::DockSpaceOverViewport();

    // todo: keep the imgui.ini file in a safe place
    static bool x_ImguiIniExists = std::filesystem::exists("imgui.ini");
    static bool x_FirstTime      = true;
    static const auto k_vpID     = apst.MainViewportId;

    if (x_FirstTime && !x_ImguiIniExists)
    {
        x_FirstTime = false;

        ImGui::DockBuilderRemoveNode(apst.MainViewportId);
        ImGui::DockBuilderAddNode(apst.MainViewportId);
        ImGui::DockBuilderSetNodeSize(apst.MainViewportId, ImGui::GetMainViewport()->Size);

        auto dockIdLeft = ImGui::DockBuilderSplitNode(apst.MainViewportId, ImGuiDir_Left, 0.25,
                                                      nullptr, &apst.MainViewportId);

        auto dockIdBottom = ImGui::DockBuilderSplitNode(apst.MainViewportId, ImGuiDir_Down, 0.3,
                                                        nullptr, &apst.MainViewportId);

        auto dockIdRight = ImGui::DockBuilderSplitNode(apst.MainViewportId, ImGuiDir_Right, 0.25,
                                                       nullptr, &apst.MainViewportId);

        ImGui::DockBuilderDockWindow("Ankush's Garage - ToolBox", dockIdLeft);
        ImGui::DockBuilderDockWindow("Console", dockIdBottom);
        ImGui::DockBuilderDockWindow("###ProjectUI", dockIdRight);
        ImGui::DockBuilderDockWindow("###TexTitle", apst.MainViewportId);

        ImGui::DockBuilderFinish(apst.MainViewportId);
    }
}

bool ImGuiCore::HandleWindowResize()
{
    ImVec2 view = ImGui::GetWindowSize();
    auto ctx    = Context::GetContext();

    if (view.x != ctx->AppState.ProjectWindowSize.x || view.y != ctx->AppState.ProjectWindowSize.y)
    {
        ctx->EventHandler.UpdateKey(Core::RESIZE_PROJECT_WINDOW, true);

        if (view.x == 0 || view.y == 0) { return false; }  // window is minimised
        ctx->AppState.ProjectWindowSize.x = view.x;
        ctx->AppState.ProjectWindowSize.y = view.y;

        Renderer::ResizeProjectTexture(view.x, view.y);
        return true;
    }

    return true;
}

void ImGuiCore::ShowToolBox()
{
    auto &apst = Context::GetContext()->AppState;

    if (ImGui::Begin("Ankush's Garage - ToolBox"))
    {
        TB_ProjectSelector();

        ImGui::SeparatorText("Controls");
        TB_ResolutionSlider();

        ImGui::SeparatorText("Data");
        Plotter::PlotAllData();
    }
    ImGui::End();
}

void ImGuiCore::TB_ProjectSelector()
{
    auto &apst = Context::GetContext()->AppState;

    // get names
    static std::vector<const char *> x_Names;
    static bool x_FirsstTime = true;

    const int projSize = Common::ProjectManager::GetProjects()->size();
    if (x_FirsstTime)
    {
        x_FirsstTime = false;
        x_Names.reserve(projSize);
        for (const auto &project: *Common::ProjectManager::GetProjects())
        {
            x_Names.push_back(project->GetName().c_str());
        }
    }
    ImGui::SeparatorText("Projects");

    // Project selector
    if (ImGui::BeginCombo("Project", x_Names[apst.ProjectIndex]))
    {
        for (int i = 0; i < projSize; i++)
        {
            const bool isSelected = (apst.ProjectIndex == i);
            if (ImGui::Selectable(x_Names[i], isSelected))
            {
                if (i != apst.ProjectIndex)
                {
                    apst.HasToChangeIndex = true;
                    apst.ProjectToBeIndex = i;
                }
            }
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void ImGuiCore::TB_ResolutionSlider()
{
    auto &apst = Context::GetContext()->AppState;

    // resolution
    static int x_Res = 100;
    ImGui::Text("Scale Resolution");
    if (ImGui::SliderInt("###yetanotherid", &x_Res, 25, 500, "%d%%"))
    {
        Context::GetContext()->RenderData.ResolutionScale = x_Res / 100.0f;
        Renderer::ResizeProjectTexture(apst.ProjectWindowSize.x, apst.ProjectWindowSize.y);
    }
}

void ImGuiCore::ShowConsole()
{
    if (ImGui::Begin("Console"))
    {
        const auto cl = ConsoleLogBuffer::GetMessages();

        if (ImGui::BeginTable("ConsoleLogWindowTable", 2,
                              ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_BordersInner))
        {
            ImGui::TableSetupColumn("Console Message", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 70);
            for (int i = cl.size() - 1; i >= 0; i--)
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
}

void ImGuiCore::ShowProjectWindow()
{
    auto &apst = Context::GetContext()->AppState;

    if (Common::BaseProject::hasUI)
    {
        if (auto imguiProject = dynamic_cast<Common::ImGuiUI *>(
                Common::ProjectManager::GetProjects()->at(apst.ProjectIndex).get()))
        {
            imguiProject->DrawUI();
        }
    }
}

void ImGuiCore::ShowProjectRendered()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});

    const auto projName = Common::ProjectManager::GetProjects()
                              ->at(Core::Context::GetContext()->AppState.ProjectIndex)
                              ->GetName();

    const auto projectWindowName = "Project - " + projName + "###TexTitle";

    auto &apst = Context::GetContext()->AppState;
    auto ctx = Context::GetContext();

    if (ImGui::Begin(projectWindowName.c_str()))
    {
        apst.projectWindowFocused = ImGui::IsWindowFocused();
        apst.projectWindowHovered = ImGui::IsWindowHovered();

        if (apst.projectWindowFocused && apst.projectWindowHovered
            && ctx->EventHandler.GetEventPressed(Core::ESC))
        {  // the user wants to get out of focus from the project screen
            ImGui::SetWindowFocus("###ProjectUI");
        }

        if (!Core::ImGuiCore::HandleWindowResize())
        {
            ImGui::End();
            ImGui::PopStyleVar();
            Garage::StopProjectUpdateLoop();
            return;
        }

        // Note to self: Moving this function down causes artifacts when resizing
        Renderer::DrawProjectToTexture();

        bind.texture = ctx->RenderData.ProjectTexture;
        bind.sampler = ctx->RenderData.ProjectSampler;

        // Draw the project to the screen ImGui window
        const auto size = ImGui::GetWindowSize();
        ImGui::Image(ImTextureID(&bind), {size.x, size.y - 19.0f});
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

}  // namespace Core
