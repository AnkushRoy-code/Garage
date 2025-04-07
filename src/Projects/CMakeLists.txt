add_subdirectory(Common)

add_library(Projects 
    SimpleExample/SimpleExample.h 
    SimpleExample/SimpleExample.cpp 
    Triangle/Triangle.h
    Triangle/Triangle.cpp 
)

add_subdirectory(Boids)
add_subdirectory(Two_Body_Simulation)

if (TRACY_PROFILE) 
    target_link_libraries(Projects PUBLIC Tracy::TracyClient)
endif()

target_link_libraries(Projects PRIVATE Projects::Common Core::Common SDL3::SDL3-shared imgui Core)
target_include_directories(Projects PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_precompile_headers(Projects REUSE_FROM Core) 
