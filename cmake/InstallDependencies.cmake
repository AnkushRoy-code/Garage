include(cmake/CPM.cmake)

CPMAddPackage(
    NAME SDL3
    VERSION 3.2.8
    URL https://github.com/libsdl-org/SDL/releases/download/release-3.2.8/SDL3-3.2.8.tar.gz
    URL_HASH SHA256=13388fabb361de768ecdf2b65e52bb27d1054cae6ccb6942ba926e378e00db03
)

CPMAddPackage(
    NAME imgui
    VERSION 1.91.8-docking
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.8-docking.tar.gz
    URL_HASH SHA256=55f5e65abea635f2a8bfa9a92cd966448a363a262cf6dead7cc662fb0ab37612
)

if(imgui_ADDED)
    file(
        GLOB
        imgui_sources
        ${imgui_SOURCE_DIR}/*.cpp
        ${imgui_SOURCE_DIR}/*.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlgpu3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlgpu3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlgpu3_shaders.h
    )
    add_library(imgui STATIC ${imgui_sources})

    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
endif()

if(TESTING)
    CPMAddPackage("gh:catchorg/Catch2@3.8.0")
endif()
