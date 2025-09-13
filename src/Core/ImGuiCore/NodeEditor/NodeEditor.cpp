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
    ImGui::SetNextItemWidth(60.0f);
    ImGui::SliderFloat("Value", &m_value, -1024, 1024);
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
    ImGui::SetNextItemWidth(60.0f);
    ImGui::Text(": %.3f", getInVal<float>("A"));
    ImGui::SetNextItemWidth(60.0f);
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
    ImGui::SetNextItemWidth(60.0f);
    ImGui::Text(": %.3f", getInVal<float>("A"));
    ImGui::SetNextItemWidth(60.0f);
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
    ImGui::SetNextItemWidth(60.0f);
    ImGui::Text(": %.3f", getInVal<float>("A"));
    ImGui::SetNextItemWidth(60.0f);
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
    ImGui::SetNextItemWidth(60.0f);
    ImGui::Text(": %.3f", getInVal<float>("A"));
    ImGui::SetNextItemWidth(60.0f);
    float b = getInVal<float>("B");
    if (b == 0)
    {
        ImGui::TextColored(ImVec4(1.f, 0.5f, 0.2f, 1.f), "Warning: divisor is zero");
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
    float val = getInVal<float>("In");
    ImGui::SetNextItemWidth(60.0f);
    ImGui::TextWrapped("Result: %.3f", val);
}

NodeEditor NodeEditor::m_editor(500, 500);

NodeEditor::NodeEditor(float d, std::size_t r) :
    BaseNode()
{
    mINF.setSize({d, d});

	// initial placement can be done here
    if (r > 0)
    {
    }

    // clang-format off
    // right-click popup to place nodes
    mINF.rightClickPopUpContent([this](ImFlow::BaseNode *node)
	{
		if (ImGui::Selectable("Result"))
			mINF.placeNodeAt<ResultNode>(ImGui::GetMousePos());
		if (ImGui::Selectable("Input"))
			mINF.placeNodeAt<FloatInputNode>(ImGui::GetMousePos());
		if (ImGui::Selectable("Add"))
			mINF.placeNodeAt<AddNode>(ImGui::GetMousePos());
		if (ImGui::Selectable("Subtract"))
			mINF.placeNodeAt<SubtractNode>(ImGui::GetMousePos());
		if (ImGui::Selectable("Multiply"))
			mINF.placeNodeAt<MultiplyNode>(ImGui::GetMousePos());
		if (ImGui::Selectable("Divide"))
			mINF.placeNodeAt<DivideNode>(ImGui::GetMousePos());
	});
    // clang-format on
}

void NodeEditor::set_size(ImVec2 d)
{
    mINF.setSize(d);
}

void NodeEditor::draw()
{
    mINF.update();
}

void NodeEditor::InitNodeEditor()
{
    // do nothing
}

void NodeEditor::ShowNodeEditor()
{
    ImGui::Begin("Node Editor", nullptr, ImGuiWindowFlags_NoTitleBar);
    m_editor.set_size({ImGui::GetWindowSize().x - 16, ImGui::GetWindowSize().y - 16});
    m_editor.draw();
    ImGui::End();
}

void NodeEditor::DestroyNodeEditor()
{
}
