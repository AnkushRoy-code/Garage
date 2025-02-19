add_library(
  Examples SimpleExample/SimpleExample.h SimpleExample/SimpleExample.cpp
           Triangle/Triangle.h Triangle/Triangle.cpp
)

target_link_libraries(Examples PRIVATE Common SDL3::SDL3-shared)
target_include_directories(Examples PRIVATE "${CMAKE_SOURCE_DIR}/src")
