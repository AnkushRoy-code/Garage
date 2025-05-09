add_library(Event STATIC EventHandler.cpp EventHandler.h)

if(TRACY_PROFILE)
    target_link_libraries(Event PUBLIC Tracy::TracyClient)
endif()

target_include_directories(Event PRIVATE "${CMAKE_SOURCE_DIR}/src")

if(SDL_STATIC)
    target_link_libraries(Event PRIVATE SDL3::SDL3-static)
else()
    target_link_libraries(Event PRIVATE SDL3::SDL3-shared)
endif()

target_link_libraries(Event PUBLIC imgui glm::glm)
target_precompile_headers(Event REUSE_FROM Core)
