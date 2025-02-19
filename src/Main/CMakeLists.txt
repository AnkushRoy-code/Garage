add_executable("${PROJECT_NAME}" Main.cpp)

target_link_libraries("${PROJECT_NAME}" PRIVATE Common Examples Utils SDL3::SDL3-shared)

target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_SOURCE_DIR}/src")
