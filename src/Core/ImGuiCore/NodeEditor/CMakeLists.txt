add_library(
    node_editor STATIC
    NodeEditor.h
    NodeEditor.cpp
)

target_include_directories(node_editor PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(node_editor PUBLIC imgui imgui_node_editor)
