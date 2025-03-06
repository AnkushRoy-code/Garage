cmake_minimum_required(VERSION 3.30)

project(Garage DESCRIPTION "A simple trial of SDL_GPU_API")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
    set(default_build_type "Debug")
    message(STATUS "Set the build type to `${default_build_type}` as none was specified.")
    set(CMAKE_BUILD_TYPE
        ${default_build_type}
        CACHE STRING "Chooce the build type." FORCE
    )
    set_property(
        CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
    )
endif()
message(STATUS "Garage Build Type: ${CMAKE_BUILD_TYPE}")

# Set the version for Garage
set(Garage_Version_Major 0)
set(Garage_Version_Minor 0)
set(Garage_Version_Patch 1)
set(Garage_Version_Status "-dev")
set(PROJECT_VERSION
    "${Garage_Version_Major}.${Garage_Version_Minor}.${Garage_Version_Patch}${Garage_Version_Status}"
)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME SDL3
    VERSION 3.2.8
    URL https://github.com/libsdl-org/SDL/releases/download/release-3.2.8/SDL3-3.2.8.tar.gz
    URL_HASH SHA256=13388fabb361de768ecdf2b65e52bb27d1054cae6ccb6942ba926e378e00db03
)

add_subdirectory(src)
