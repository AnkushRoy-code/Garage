add_library(core_common STATIC Common.h Common.cpp)
add_library(Core::Common ALIAS core_common)

if (TRACY_PROFILE) 
    target_link_libraries(core_common PUBLIC Tracy::TracyClient)
endif()

target_include_directories(core_common PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(core_common PRIVATE SDL3::SDL3-shared imgui glm::glm)
target_precompile_headers(core_common REUSE_FROM Core) 
