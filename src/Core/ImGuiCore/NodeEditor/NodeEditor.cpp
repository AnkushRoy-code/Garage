#include "NodeEditor.h"

#include <imgui.h>
#include <imgui_node_editor.h>

namespace ed = ax::NodeEditor;

ed::EditorContext *Core::NodeEditor::m_Context {};

void Core::NodeEditor::InitNodeEditor()
{
    ed::Config config;
    config.SettingsFile = "Simple.json";
    m_Context           = ed::CreateEditor(&config);
}

void Core::NodeEditor::ShowNodeEditor()
{
    ImGui::Begin("Node Editor");
    {
        ed::SetCurrentEditor(m_Context);
        ed::Begin("My Editor", ImVec2(0.0, 0.0f));
        {
            int uniqueId = 1;
            // Start drawing nodes.
            ed::BeginNode(uniqueId++);
            {
                ImGui::Text("Node A");
                ed::BeginPin(uniqueId++, ed::PinKind::Input);
                {
                    ImGui::Text("-> In");
                }
                ed::EndPin();
                ImGui::SameLine();
                ed::BeginPin(uniqueId++, ed::PinKind::Output);
                {
                    ImGui::Text("Out ->");
                }
                ed::EndPin();
            }
            ed::EndNode();
        }
        ed::End();
        ed::SetCurrentEditor(nullptr);
    }
    ImGui::End();
}

void Core::NodeEditor::DestroyNodeEditor()
{
    ed::DestroyEditor(m_Context);
}
