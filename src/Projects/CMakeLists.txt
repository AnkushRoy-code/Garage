add_library(Projects 
    SimpleExample/SimpleExample.h 
    SimpleExample/SimpleExample.cpp 
    Triangle/Triangle.h
    Triangle/Triangle.cpp 
)

add_subdirectory(Particle)

if (TRACY_PROFILE) 
    target_link_libraries(Projects PUBLIC Tracy::TracyClient)
endif()

target_link_libraries(Projects PRIVATE Common SDL3::SDL3-shared Particle)
target_include_directories(Projects PRIVATE "${CMAKE_SOURCE_DIR}/src" ${imgui_SOURCE_DIR})
