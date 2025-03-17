add_library(Common STATIC BaseProject.h BaseProject.cpp RegisterProjects.cpp Common.h Common.cpp SDL_Exception.h)

if (TRACY_PROFILE) 
    target_link_libraries(Common PUBLIC Tracy::TracyClient)
endif()

target_include_directories(Common PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(Common PRIVATE SDL3::SDL3-shared imgui)
