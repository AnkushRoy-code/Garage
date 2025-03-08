add_executable("${PROJECT_NAME}" Main.cpp)

target_link_libraries("${PROJECT_NAME}" PRIVATE Common Projects Utils SDL3::SDL3-shared imgui)

if(TRACY_PROFILE)
    target_link_libraries(${PROJECT_NAME} PUBLIC Tracy::TracyClient)
endif()

target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_SOURCE_DIR}/src")

if(WIN32)
    if(MSVC)
        set_property(
            DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}
        )
    endif()

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:SDL3::SDL3>"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
endif()

add_custom_command(
    TARGET "${PROJECT_NAME}"
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
)
