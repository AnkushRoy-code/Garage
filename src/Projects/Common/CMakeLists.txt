add_library(projects_common STATIC BaseProject.h BaseProject.cpp RegisterProjects.cpp)
add_library(Projects::Common ALIAS projects_common)

if(TRACY_PROFILE)
    target_link_libraries(Projects::Common PUBLIC Tracy::TracyClient)
endif()

target_include_directories(projects_common PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(projects_common PRIVATE SDL3::SDL3-shared imgui glm::glm Boids Two_Body_Simulation)
