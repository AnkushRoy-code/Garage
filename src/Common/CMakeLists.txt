add_library(Common STATIC BaseProject.h BaseProject.cpp Context.h Context.cpp RegisterProjects.cpp Common.h Common.cpp SDL_Exception.h)

target_include_directories(Common PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(Common PRIVATE Projects SDL3::SDL3-shared)
