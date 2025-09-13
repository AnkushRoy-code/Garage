#ifndef INCLUDE_NODEEDITOR_NODEEDITOR_H_
#define INCLUDE_NODEEDITOR_NODEEDITOR_H_

#include "imgui.h"
#include <ImNodeFlow.h>

namespace Core
{

class FloatInputNode : public ImFlow::BaseNode
{
  public:
    FloatInputNode();
    void draw() override;

  private:
    float m_value = 0.0;
};

class AddNode : public ImFlow::BaseNode
{
  public:
    AddNode();
    void draw() override;
};

class SubtractNode : public ImFlow::BaseNode
{
  public:
    SubtractNode();
    void draw() override;
};

class MultiplyNode : public ImFlow::BaseNode
{
  public:
    MultiplyNode();
    void draw() override;
};

class DivideNode : public ImFlow::BaseNode
{
  public:
    DivideNode();
    void draw() override;
};

class ResultNode : public ImFlow::BaseNode
{
  public:
    ResultNode();
    void draw() override;
};

struct NodeEditor : ImFlow::BaseNode
{

    static void ShowNodeEditor();
    static void InitNodeEditor();
    static void DestroyNodeEditor();

    NodeEditor(float d, std::size_t r);

    static void set_size(ImVec2 d);
    void draw() override;

  private:
    static ImFlow::ImNodeFlow m_INF;
    static NodeEditor m_editor;
};

}  // namespace Core

#endif  // INCLUDE_NODEEDITOR_NODEEDITOR_H_
