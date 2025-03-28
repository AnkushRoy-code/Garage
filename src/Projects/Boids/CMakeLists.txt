add_library(
    Boids
    src/Main.cpp
    src/Main.h
)

if(TRACY_PROFILE)
    target_link_libraries(Boids PUBLIC Tracy::TracyClient)
endif()

target_link_libraries(Boids PRIVATE imgui)
target_include_directories(Boids PRIVATE "${CMAKE_SOURCE_DIR}/src" ${imgui_SOURCE_DIR})
