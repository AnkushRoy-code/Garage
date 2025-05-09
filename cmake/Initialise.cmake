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
set(Garage_Version_Minor 2)
set(Garage_Version_Patch 6)
set(PROJECT_VERSION "${Garage_Version_Major}.${Garage_Version_Minor}.${Garage_Version_Patch}")
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")
