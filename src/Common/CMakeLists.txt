add_library(Common STATIC Context.h Context.cpp RegisterExamples.cpp)

target_link_libraries(Common PRIVATE Examples SDL3::SDL3-shared)
