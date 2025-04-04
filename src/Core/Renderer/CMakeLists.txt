add_library(Renderer STATIC Renderer.h Renderer.cpp)

target_include_directories(Renderer PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(Renderer PRIVATE SDL3::SDL3-shared imgui)
