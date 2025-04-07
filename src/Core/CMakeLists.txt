add_library(Core STATIC Context.h Context.cpp Console.h Console.cpp EventHandler.h EventHandler.cpp)

if(TRACY_PROFILE)
    target_link_libraries(Core PUBLIC Tracy::TracyClient)
endif()

target_include_directories(Core PRIVATE "${CMAKE_SOURCE_DIR}/src")

target_precompile_headers(Core PRIVATE Common/pch.h)

target_link_libraries(Core PRIVATE CoreImGui Renderer Projects)

target_link_libraries(Core PUBLIC SDL3::SDL3-shared imgui glm::glm)

add_subdirectory(ImGuiCore)
add_subdirectory(Renderer)
add_subdirectory(Common)
