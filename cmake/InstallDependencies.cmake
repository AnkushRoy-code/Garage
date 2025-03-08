include(cmake/CPM.cmake)

# I use tar.gz archives and sha256 instead of git because when I need to test my code in windows git
# clone drains all my internet. I have limited daily. Also GIT_SHALLOW doesn't work how it sould
# even after I give it proper tags or even commit.

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

# ImGui doesn't have any CMakeLists.txt so we create our own. If you're not using SDL3 as your
# backend you might like to use this:
# https://github.com/cpm-cmake/CPM.cmake/wiki/More-Snippets#imgui, it is much more simpler and easier
# than what I am doing. Hopefully it adds SDL3 & SDL3_GPU support soon.

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
    target_link_libraries(imgui PUBLIC SDL3::SDL3-shared)
endif()

if(TESTING)
    CPMAddPackage("gh:catchorg/Catch2@3.8.0") # Nothing rn
endif()

if(TRACY_PROFILE_COMPATIBILITY) # For my old lappy
    option(TRACY_ENABLE "" ON)
    CPMAddPackage(
        NAME Tracy
        VERSION 
        URL https://github.com/wolfpld/tracy/archive/refs/tags/v0.11.1.tar.gz
        URL_HASH SHA256=2c11ca816f2b756be2730f86b0092920419f3dabc7a7173829ffd897d91888a1
        OPTIONS "TRACY_TIMER_FALLBACK ON"
    ) 
endif()

if(TRACY_PROFILE)
    option(TRACY_ENABLE "" ON)
    CPMAddPackage(
        NAME Tracy
        VERSION 
        URL https://github.com/wolfpld/tracy/archive/refs/tags/v0.11.1.tar.gz
        URL_HASH SHA256=2c11ca816f2b756be2730f86b0092920419f3dabc7a7173829ffd897d91888a1
    ) 
endif()
