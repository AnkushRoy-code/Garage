add_executable("${PROJECT_NAME}" Main.cpp MainImpl.cpp Main.h Flags.h Flags.cpp)

if(TRACY_PROFILE)
    target_link_libraries(${PROJECT_NAME} PUBLIC Tracy::TracyClient)
endif()

if(SDL_STATIC)
    target_link_libraries("${PROJECT_NAME}" PRIVATE SDL3::SDL3-static)
else()
    target_link_libraries("${PROJECT_NAME}" PRIVATE SDL3::SDL3-shared)
endif()

target_link_libraries("${PROJECT_NAME}" PRIVATE Projects::Common Utils imgui Core cxxopts)
target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_precompile_headers("${PROJECT_NAME}" REUSE_FROM Core)

# Copy shared lib(s) or res if needed
if(WIN32)
    if(MSVC)
        set_property(
            DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}
        )

        # no console in windows
        set_target_properties(
            "${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS
                                               "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
        )
    endif()

    if(!SDL_STATIC)
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:SDL3::SDL3>"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        )
    endif()

    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/res/Images/Logo/AnkushGarage.ico
                ${CMAKE_CURRENT_BINARY_DIR}/AnkushGarage.ico
    )

    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/res/icon.rc")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

if(!RELEASE_BUILD)
    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
    )
endif()

if(UNIX AND NOT APPLE)
    configure_file(
        ${CMAKE_SOURCE_DIR}/res/garage.desktop.in ${CMAKE_BINARY_DIR}/garage.desktop @ONLY
    )
    install(FILES ${CMAKE_BINARY_DIR}/garage.desktop DESTINATION share/applications)

    # install icon
    install(FILES ${CMAKE_SOURCE_DIR}/res/Images/Logo/AnkushGarage.png
            DESTINATION share/icons/hicolor/256x256/apps
    )
endif()
