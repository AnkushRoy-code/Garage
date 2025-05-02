add_library(
    projects_common STATIC BaseProject.h BaseProject.cpp RegisterProjects.cpp Camera.h Camera.cpp
)
add_library(Projects::Common ALIAS projects_common)

if(TRACY_PROFILE)
    target_link_libraries(Projects::Common PUBLIC Tracy::TracyClient)
endif()

if(EMSCRIPTEN)
    target_link_libraries(projects_common PRIVATE SDL3::SDL3-static)
else()
    target_link_libraries(projects_common PRIVATE SDL3::SDL3-shared)
endif()

target_include_directories(projects_common PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(projects_common PRIVATE imgui glm::glm Boids N_Body_Simulation)
