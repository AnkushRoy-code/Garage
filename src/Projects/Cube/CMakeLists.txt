add_library(Cube STATIC Main.cpp Main.h)

if(TRACY_PROFILE)
    target_link_libraries(Cube PUBLIC Tracy::TracyClient)
endif()

target_include_directories(Cube PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(Cube PRIVATE Core)
target_precompile_headers(Cube REUSE_FROM Core) 
