add_library(Core STATIC Context.h Context.cpp Console.h Console.cpp EventHandler.h EventHandler.cpp)
add_subdirectory(ImGuiCore)
add_subdirectory(Renderer)

if(TRACY_PROFILE)
    target_link_libraries(Core PUBLIC Tracy::TracyClient)
endif()

target_include_directories(Core PRIVATE "${CMAKE_SOURCE_DIR}/src" ${imgui_SOURCE_DIR})
target_link_libraries(Core PRIVATE Projects SDL3::SDL3-shared imgui CoreImGui Renderer)
