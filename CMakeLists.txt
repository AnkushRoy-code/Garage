cmake_minimum_required(VERSION 3.30)

project(GPU_API_TRY DESCRIPTION "A simple trial of SDL_GPU_API")

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
message(STATUS "GPU_API_TRY Build Type: ${CMAKE_BUILD_TYPE}")

# Set the version for GPU_API_TRY
set(GPU_API_TRY_Version_Major 0)
set(GPU_API_TRY_Version_Minor 0)
set(GPU_API_TRY_Version_Patch 1)
set(GPU_API_TRY_Version_Status "-dev")
set(PROJECT_VERSION
    "${GPU_API_TRY_Version_Major}.${GPU_API_TRY_Version_Minor}.${GPU_API_TRY_Version_Patch}${GPU_API_TRY_Version_Status}"
)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

# Copy the res directory to the build directory
set(RESOURCES_OUTPUT_DIR "${CMAKE_BINARY_DIR}/res")
add_custom_target(
  CopyResources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${RESOURCES_OUTPUT_DIR}
  COMMENT "Copying resources to the build directory."
)

include(cmake/CPM.cmake)

CPMAddPackage("gh:libsdl-org/SDL#release-3.2.4")

add_subdirectory(src)
