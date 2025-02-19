add_library(Common STATIC Context.h Context.cpp RegisterExamples.cpp Common.h Common.cpp SDL_Exception.h)

target_include_directories(Common PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(Common PRIVATE Examples SDL3::SDL3-shared)
