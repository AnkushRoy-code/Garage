# Why is it named CoreImGui instead of ImGui? Because I got into problems with
# name conflicts in Mac and Windows build.

add_library(CoreImGui STATIC ImGuiCore.h ImGuiCore.cpp)

target_include_directories(CoreImGui PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(CoreImGui PRIVATE imgui implot)
