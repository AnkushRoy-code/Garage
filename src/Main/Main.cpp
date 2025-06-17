#include "Main.h"
#include "Main/Flags.h"
#include "imgui.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>  // only need in main.cpp

#include "Core/Common/pch.h"

#include <imgui_node_editor.h>

#include "Core/Common/Common.h"
#include "Core/Context.h"
#include "Core/Event/EventHandler.h"
#include "Core/ImGuiCore/ImGuiCore.h"
#include "Core/ImGuiCore/FPSTracker.h"

#include "Projects/Common/BaseProject.h"

#include "Utils/Timer.h"

namespace ed = ax::NodeEditor;
ed::EditorContext *ctx_e {};
bool initialised = false;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (HandleFLags::HandleFlags(argc, argv))
    {
        Garage::InitCoreSystems();
        ed::Config config;
        config.SettingsFile = "Simple.json";
        ctx_e               = ed::CreateEditor(&config);
        initialised         = true;
        return SDL_APP_CONTINUE;
    }
    else
        return SDL_APP_SUCCESS;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    PROFILE_SCOPE;

    if (Garage::WindowMinimised) { return SDL_APP_CONTINUE; }

    ImGui_ImplSDL3_ProcessEvent(event);
    return Core::Context::GetContext()->EventHandler.HandleEvents(event);
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    PROFILE_SCOPE;
    Garage::InitiateProjectUpdateLoop();

    // no need to draw if window is minimised. But we sure need to update the state.
    if (Garage::WindowMinimised)
    {
        Garage::StopProjectUpdateLoop();
        return SDL_APP_CONTINUE;
    }

    Tracker::RegisterPoints();

    {
        static float renderTime = 0.0f;
        Tracker::AddRenderFPSPointQueue(renderTime);
        Timer temporaryUselessVariable(renderTime);

        Core::ImGuiCore::Update();

        ImGui::Begin("newWind");
            ed::SetCurrentEditor(ctx_e);
            ed::Begin("My Editor", ImVec2(0.0, 0.0f));
            int uniqueId = 1;
            // Start drawing nodes.
            ed::BeginNode(uniqueId++);
                ImGui::Text("Node A");
                ed::BeginPin(uniqueId++, ed::PinKind::Input);
                    ImGui::Text("-> In");
                ed::EndPin();
                ImGui::SameLine();
                ed::BeginPin(uniqueId++, ed::PinKind::Output);
                    ImGui::Text("Out ->");
                ed::EndPin();
            ed::EndNode();
            ed::End();
            ed::SetCurrentEditor(nullptr);
        ImGui::End();

        Core::ImGuiCore::Draw();
    }

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Garage::StopProjectUpdateLoop();
    Garage::ChangeProject();

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (initialised)
    {
        ed::DestroyEditor(ctx_e);
    }
    // Do stuff when needed
}
