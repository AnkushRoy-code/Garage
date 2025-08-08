cmake_minimum_required(VERSION 3.30..4)

project(Garage DESCRIPTION "A simple trial of SDL_GPU_API")

include(cmake/Initialise.cmake)

message(STATUS "GARAGE: Installing Dependencies...")
include(cmake/InstallDependencies.cmake)

add_subdirectory(src)

include(cmake/Release.cmake)

if(TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
