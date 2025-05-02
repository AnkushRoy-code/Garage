include(cmake/CPM.cmake)

# I use tar.gz archives and sha256 instead of git because when I need to test my code in windows git
# clone drains all my internet. I have limited daily. Also GIT_SHALLOW doesn't work how it sould
# even after I give it proper tags or even commit.

# ############################ SDL ###############################
CPMAddPackage(
    NAME SDL3
    VERSION 3.2.8
    URL https://github.com/libsdl-org/SDL/releases/download/release-3.2.8/SDL3-3.2.8.tar.gz
    URL_HASH SHA256=13388fabb361de768ecdf2b65e52bb27d1054cae6ccb6942ba926e378e00db03
)

# ############################ ImGui ###############################
CPMAddPackage(
    NAME imgui
    VERSION 1.91.8-docking
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.8-docking.tar.gz
    URL_HASH SHA256=55f5e65abea635f2a8bfa9a92cd966448a363a262cf6dead7cc662fb0ab37612
)

# ImGui doesn't have any CMakeLists.txt so we create our own. If you're not using SDL3 as your
# backend you might like to use this:
# https://github.com/cpm-cmake/CPM.cmake/wiki/More-Snippets#imgui, it is much more simpler and
# easier than what I am doing. Hopefully it adds SDL3 & SDL3_GPU support soon.

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
    if(EMSCRIPTEN)
        target_link_libraries(imgui PRIVATE SDL3::SDL3-static)
    else()
        target_link_libraries(imgui PRIVATE SDL3::SDL3-shared)
    endif()
endif()

CPMAddPackage(
    NAME implot
    VERSION 1.92.x
    URL https://github.com/epezent/implot/archive/3da8bd34299965d3b0ab124df743fe3e076fa222.zip
    URL_HASH SHA256=e9b439a94aa6f09fcf12ed9c2cd784efe5e99e96242122aa683e690b29f7a59f
)

if(implot_ADDED)
    file(
        GLOB
        implot_sources
        ${implot_SOURCE_DIR}/implot.h
        ${implot_SOURCE_DIR}/implot_internal.h
        ${implot_SOURCE_DIR}/implot.cpp
        ${implot_SOURCE_DIR}/implot_items.cpp
        ${implot_SOURCE_DIR}/implot_demo.cpp
    )
    add_library(implot STATIC ${implot_sources})

    target_include_directories(implot PUBLIC ${imgui_SOURCE_DIR} ${implot_SOURCE_DIR})
    if(EMSCRIPTEN)
        target_link_libraries(implot PUBLIC SDL3::SDL3-static imgui)
    else()
        target_link_libraries(implot PUBLIC SDL3::SDL3-shared imgui)
    endif()
endif()

# ############################ Catch2 ###############################
if(TESTING)
    CPMAddPackage("gh:catchorg/Catch2@3.8.0") # Nothing rn
endif()

# ############################ Tracy Profiler ###############################
if(TRACY_PROFILE_COMPATIBILITY) # For my old lappy
    option(TRACY_ENABLE "" ON)
    CPMAddPackage(
        NAME Tracy
        VERSION URL https://github.com/wolfpld/tracy/archive/refs/tags/v0.11.1.tar.gz
        URL_HASH SHA256=2c11ca816f2b756be2730f86b0092920419f3dabc7a7173829ffd897d91888a1
        OPTIONS "TRACY_TIMER_FALLBACK ON"
    )
endif()

if(TRACY_PROFILE)
    option(TRACY_ENABLE "" ON)
    CPMAddPackage(
        NAME Tracy
        VERSION URL https://github.com/wolfpld/tracy/archive/refs/tags/v0.11.1.tar.gz
        URL_HASH SHA256=2c11ca816f2b756be2730f86b0092920419f3dabc7a7173829ffd897d91888a1
    )
endif()

# ########################### miniaudio ###############################
# CPMAddPackage( NAME miniaudio VERSION 0.11.22 URL
# https://github.com/mackron/miniaudio/archive/refs/tags/0.11.22.tar.gz URL_HASH
# SHA256=bcb07bfb27e6fa94d34da73ba2d5642d4940b208ec2a660dbf4e52e6b7cd492f )

# ########################### GLM ###############################
CPMAddPackage(
    NAME glm
    VERSION 1.0.1
    URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.tar.gz
    URL_HASH SHA256=9f3174561fd26904b23f0db5e560971cbf9b3cbda0b280f04d5c379d03bf234c
)

# ########################### EnTT ###############################

# CPMAddPackage( NAME EnTT VERSION 3.15.0 URL
# https://github.com/skypjack/entt/archive/refs/tags/v3.15.0.tar.gz URL_HASH
# SHA256=01466fcbf77618a79b62891510c0bbf25ac2804af5751c84982b413852234d66 # EnTT's CMakeLists screws
# with configuration options DOWNLOAD_ONLY True )

# if (EnTT_ADDED) add_library(EnTT INTERFACE) target_include_directories(EnTT INTERFACE
# ${EnTT_SOURCE_DIR}/src) endif()
