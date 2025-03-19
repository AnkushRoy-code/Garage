add_library(
    Particle
    src/ImGuiMenu.cpp
    src/ImGuiMenu.h
    src/Main.cpp
    src/Main.h
    src/ParticleContainer.cpp
    src/ParticleContainer.h
    src/ParticleController.cpp
    src/ParticleController.h
    src/ParticleData.cpp
    src/ParticleData.h
    src/ParticleDefaultData.cpp
    src/ParticleDefaultData.h
)

if(TRACY_PROFILE)
    target_link_libraries(Particle PUBLIC Tracy::TracyClient)
endif()

target_link_libraries(Particle PRIVATE imgui)
target_include_directories(Particle PRIVATE "${CMAKE_SOURCE_DIR}/src" ${imgui_SOURCE_DIR})
