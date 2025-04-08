add_library(Two_Body_Simulation STATIC src/Main.cpp src/Main.h src/Particle.h src/Particle.cpp)

if(TRACY_PROFILE)
    target_link_libraries(Two_Body_Simulation PUBLIC Tracy::TracyClient)
endif()

target_include_directories(Two_Body_Simulation PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(Two_Body_Simulation PRIVATE Core)
target_precompile_headers(Two_Body_Simulation REUSE_FROM Core)
