#include "NodeEditor.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImNodeFlow.h>

using namespace Core;

FloatInputNode::FloatInputNode()
{
    setTitle("Decimal");
    setStyle(ImFlow::NodeStyle::brown());

    ImFlow::BaseNode::addOUT<float>("Out", nullptr)
        ->behaviour([this]() -> float
                    {
                        return m_value;
                    });
}

void FloatInputNode::draw()
{
    constexpr float width = 60.0f;
    ImGui::SetNextItemWidth(width);
    const float limit = 128.0f;
    ImGui::SliderFloat("Value", &m_value, -limit, limit);
}

AddNode::AddNode()
{
    setTitle("Add");
    setStyle(ImFlow::NodeStyle::cyan());
    ImFlow::BaseNode::addIN<float>("A", float(0), ImFlow::ConnectionFilter::SameType());
    ImFlow::BaseNode::addIN<float>("B", float(0), ImFlow::ConnectionFilter::SameType());
    ImFlow::BaseNode::addOUT<float>("Out", nullptr)
        ->behaviour([this]() -> float
                    {
                        return this->getInVal<float>("A") + this->getInVal<float>("B");
                    });
}

void AddNode::draw()
{
    constexpr float width = 60.0f;
    ImGui::SetNextItemWidth(width);
    ImGui::Text(": %.3f", getInVal<float>("A"));
    ImGui::SetNextItemWidth(width);
    ImGui::Text(": %.3f", getInVal<float>("B"));
}

SubtractNode::SubtractNode()
{
    setTitle("Subtract");
    setStyle(ImFlow::NodeStyle::brown());
    ImFlow::BaseNode::addIN<float>("A", float(0), ImFlow::ConnectionFilter::SameType());
    ImFlow::BaseNode::addIN<float>("B", float(0), ImFlow::ConnectionFilter::SameType());
    ImFlow::BaseNode::addOUT<float>("Out", nullptr)
        ->behaviour([this]() -> float
                    {
                        return this->getInVal<float>("A") - this->getInVal<float>("B");
                    });
}

void SubtractNode::draw()
{
    constexpr float width = 60.0f;
    ImGui::SetNextItemWidth(width);
    ImGui::Text(": %.3f", getInVal<float>("A"));
    ImGui::SetNextItemWidth(width);
    ImGui::Text(": %.3f", getInVal<float>("B"));
}

MultiplyNode::MultiplyNode()
{
    setTitle("Multiply");
    setStyle(ImFlow::NodeStyle::red());
    ImFlow::BaseNode::addIN<float>("A", float(0), ImFlow::ConnectionFilter::SameType());
    ImFlow::BaseNode::addIN<float>("B", float(0), ImFlow::ConnectionFilter::SameType());
    ImFlow::BaseNode::addOUT<float>("Out", nullptr)
        ->behaviour([this]() -> float
                    {
                        return this->getInVal<float>("A") * this->getInVal<float>("B");
                    });
}

void MultiplyNode::draw()
{
    constexpr float width = 60.0f;
    ImGui::SetNextItemWidth(width);
    ImGui::Text(": %.3f", getInVal<float>("A"));
    ImGui::SetNextItemWidth(width);
    ImGui::Text(": %.3f", getInVal<float>("B"));
}

DivideNode::DivideNode()
{
    setTitle("Divide");
    setStyle(ImFlow::NodeStyle::cyan());
    ImFlow::BaseNode::addIN<float>("A", float(0), ImFlow::ConnectionFilter::SameType());
    ImFlow::BaseNode::addIN<float>("B", float(0), ImFlow::ConnectionFilter::SameType());
    ImFlow::BaseNode::addOUT<float>("Out", nullptr)
        ->behaviour([this]() -> float
                    {
                        float a = this->getInVal<float>("A");
                        float b = this->getInVal<float>("B");
                        if (b == float(0)) return std::numeric_limits<float>::infinity();
                        return a / b;
                    });
}

void DivideNode::draw()
{
    constexpr float width = 60.0f;
    ImGui::SetNextItemWidth(width);
    ImGui::Text(": %.3f", getInVal<float>("A"));
    ImGui::SetNextItemWidth(width);
    float b = getInVal<float>("B");
    if (b == 0)
    {
        const ImVec4 col = {1.f, 0.5f, 0.2f, 1.f};
        ImGui::TextColored(col, "Warning: divisor is zero");
    }
    else
        ImGui::Text(": %.3f", b);
}

ResultNode::ResultNode()
{
    setTitle("Result");
    setStyle(ImFlow::NodeStyle::green());
    ImFlow::BaseNode::addIN<float>("In", 0.0, ImFlow::ConnectionFilter::SameType());
    // no outputs
}

void ResultNode::draw()
{
    constexpr float width = 60.0f;
    float val             = getInVal<float>("In");
    ImGui::SetNextItemWidth(width);
    ImGui::Text("Result: %.3f", val);
}

NodeEditor NodeEditor::m_editor(500, 500);
ImFlow::ImNodeFlow NodeEditor::m_INF;

NodeEditor::NodeEditor(float d, std::size_t r) :
    BaseNode()
{
    m_INF.setSize({d, d});

    // initial placement can be done here
    if (r > 0)
    {
    }
}

void NodeEditor::set_size(ImVec2 d)
{
    m_INF.setSize(d);
}

void NodeEditor::draw()
{
    m_INF.update();
}

void NodeEditor::InitNodeEditor()
{
    // do nothing
}

void NodeEditor::ShowNodeEditor()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
    ImGui::Begin("Node Editor", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::PopStyleVar();
    constexpr float vPadding = 19.0f;
    set_size({ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - vPadding});
    m_editor.draw();

    const ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoReopen | ImGuiPopupFlags_NoOpenOverExistingPopup;
    if (ImGui::BeginPopupContextItem("Node Editor", flags))
    {
        if (ImGui::MenuItem("Result"))
            m_INF.placeNodeAt<ResultNode>(ImGui::GetMousePos());
        if (ImGui::MenuItem("Input"))
            m_INF.placeNodeAt<FloatInputNode>(ImGui::GetMousePos());
        if (ImGui::MenuItem("Add"))
            m_INF.placeNodeAt<AddNode>(ImGui::GetMousePos());
        if (ImGui::MenuItem("Subtract"))
            m_INF.placeNodeAt<SubtractNode>(ImGui::GetMousePos());
        if (ImGui::MenuItem("Multiply"))
            m_INF.placeNodeAt<MultiplyNode>(ImGui::GetMousePos());
        if (ImGui::MenuItem("Divide"))
            m_INF.placeNodeAt<DivideNode>(ImGui::GetMousePos());
        ImGui::EndPopup();
    }
    ImGui::End();
}

void NodeEditor::DestroyNodeEditor()
{
}
