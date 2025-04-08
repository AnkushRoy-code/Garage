add_library(N_Body_Simulation STATIC src/Main.cpp src/Main.h src/Particle.h src/Particle.cpp)

if(TRACY_PROFILE)
    target_link_libraries(N_Body_Simulation PUBLIC Tracy::TracyClient)
endif()

target_include_directories(N_Body_Simulation PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(N_Body_Simulation PRIVATE Core Utils)
target_precompile_headers(N_Body_Simulation REUSE_FROM Core)
