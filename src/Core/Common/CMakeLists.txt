add_library(core_common STATIC Common.h Common.cpp)
add_library(Core::Common ALIAS core_common)

if(TRACY_PROFILE)
    target_link_libraries(core_common PUBLIC Tracy::TracyClient)
endif()

target_include_directories(core_common PRIVATE "${CMAKE_SOURCE_DIR}/src")

if(SDL_STATIC)
    target_link_libraries(core_common PRIVATE SDL3::SDL3-static)
else()
    target_link_libraries(core_common PRIVATE SDL3::SDL3-shared)
endif()

target_link_libraries(core_common PRIVATE imgui glm::glm)
target_precompile_headers(core_common REUSE_FROM Core)
