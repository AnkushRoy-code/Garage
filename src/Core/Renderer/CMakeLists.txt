add_library(Renderer STATIC Renderer.h Renderer.cpp)

target_include_directories(Renderer PRIVATE "${CMAKE_SOURCE_DIR}/src")

if(EMSCRIPTEN)
    target_link_libraries(Renderer PRIVATE SDL3::SDL3-static)
else()
    target_link_libraries(Renderer PRIVATE SDL3::SDL3-shared)
endif()

target_link_libraries(Renderer PRIVATE imgui glm::glm)
