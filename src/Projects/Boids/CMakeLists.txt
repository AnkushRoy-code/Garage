add_library(
    Boids
    STATIC
    src/Main.cpp
    src/Main.h
    src/Boids.cpp
    src/Boids.h
)

if(TRACY_PROFILE)
    target_link_libraries(Boids PUBLIC Tracy::TracyClient)
endif()

target_include_directories(Boids PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(Boids PRIVATE Core)
target_precompile_headers(Boids REUSE_FROM Core) 
