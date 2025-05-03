add_subdirectory(Common)

add_library(
    Projects SimpleExample/SimpleExample.h SimpleExample/SimpleExample.cpp Triangle/Triangle.h
             Triangle/Triangle.cpp
)

add_subdirectory(Boids)
add_subdirectory(N_Body_Simulation)

if(TRACY_PROFILE)
    target_link_libraries(Projects PUBLIC Tracy::TracyClient)
endif()

if(SDL_STATIC)
    target_link_libraries(Projects PRIVATE SDL3::SDL3-static)
else()
    target_link_libraries(Projects PRIVATE SDL3::SDL3-shared)
endif()

target_link_libraries(Projects PRIVATE Projects::Common Core::Common imgui Core)
target_include_directories(Projects PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_precompile_headers(Projects REUSE_FROM Core)
