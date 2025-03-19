add_library(Renderer STATIC Renderer.h Renderer.cpp)

target_include_directories(Renderer PRIVATE "${CMAKE_SOURCE_DIR}/src" ${imgui_SOURCE_DIR})
target_link_libraries(Renderer PRIVATE SDL3::SDL3-shared)
