cmake_minimum_required(VERSION 3.30)

project(Garage DESCRIPTION "A simple trial of SDL_GPU_API")

include(cmake/Initialise.cmake)
include(cmake/InstallDependencies.cmake)

add_subdirectory(src)

include(cmake/Release.cmake)
