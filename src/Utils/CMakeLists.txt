add_library(Utils STATIC Time.h Time.cpp)

target_include_directories(Utils PRIVATE "${CMAKE_SOURCE_DIR}/src")

if (TRACY_PROFILE) 
    target_link_libraries(Utils PUBLIC Tracy::TracyClient)
endif()
