add_library(
    Particle
    src/Main.cpp
    src/Particle.cpp
    src/Particle.h
    src/ParticleController.cpp
    src/ParticleController.h
    src/ImGuiMenu.cpp
    src/ImGuiMenu.h
)

if(TRACY_PROFILE)
    target_link_libraries(Particle PUBLIC Tracy::TracyClient)
endif()

target_link_libraries(Particle PRIVATE imgui)
target_include_directories(Particle PRIVATE "${CMAKE_SOURCE_DIR}/src" ${imgui_SOURCE_DIR})
