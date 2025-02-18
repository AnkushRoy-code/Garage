add_library(
  Examples SimpleExample/SimpleExample.h SimpleExample/SimpleExample.cpp
           SimpleExample2/SimpleExample2.h SimpleExample2/SimpleExample2.cpp
)

target_include_directories(Examples PRIVATE "${CMAKE_SOURCE_DIR}")
