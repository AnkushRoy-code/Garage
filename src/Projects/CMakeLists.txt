add_library(Projects 
    SimpleExample/SimpleExample.h 
    SimpleExample/SimpleExample.cpp 
    Triangle/Triangle.h
    Triangle/Triangle.cpp 
    SimpleImGui/SimpleImGui.h 
    SimpleImGui/SimpleImGui.cpp
)

target_link_libraries(Projects PRIVATE Common SDL3::SDL3-shared)
target_include_directories(Projects PRIVATE "${CMAKE_SOURCE_DIR}/src" ${imgui_SOURCE_DIR})
