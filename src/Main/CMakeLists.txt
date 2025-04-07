add_executable("${PROJECT_NAME}" Main.cpp)

if(TRACY_PROFILE)
    target_link_libraries(${PROJECT_NAME} PUBLIC Tracy::TracyClient)
endif()

target_link_libraries("${PROJECT_NAME}" PRIVATE Projects::Common Utils SDL3::SDL3-shared imgui Core)
target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_SOURCE_DIR}/src")

target_precompile_headers("${PROJECT_NAME}" REUSE_FROM Core) 

# Copy shared lib(s) or res if needed
if(WIN32)
    if(MSVC)
        set_property(
            DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}
        )
    endif()

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:SDL3::SDL3>"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )

    # no console in windows
    set_target_properties(
        "${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
endif()

add_custom_command(
    TARGET "${PROJECT_NAME}"
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
)
