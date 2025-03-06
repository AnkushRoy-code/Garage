add_executable("${PROJECT_NAME}" Main.cpp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries("${PROJECT_NAME}" PRIVATE Common Examples Utils SDL3::SDL3-shared)

target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_SOURCE_DIR}/src")

add_custom_command(
    TARGET "${PROJECT_NAME}" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE_DIR:SDL3::SDL3>"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

add_custom_command(
    TARGET "${PROJECT_NAME}" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
)
