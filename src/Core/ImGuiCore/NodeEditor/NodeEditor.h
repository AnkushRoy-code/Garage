#ifndef INCLUDE_NODEEDITOR_NODEEDITOR_H_
#define INCLUDE_NODEEDITOR_NODEEDITOR_H_

#include <imgui_node_editor.h>

namespace Core {

class NodeEditor {
public:
    static void ShowNodeEditor();
    static void InitNodeEditor();
    static void DestroyNodeEditor();
private:
    static ax::NodeEditor::EditorContext * m_Context;
};


}

#endif  // INCLUDE_NODEEDITOR_NODEEDITOR_H_
