# Set the version for Garage
find_package(Git)

if(Git_FOUND AND GIT_EXECUTABLE)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags
        OUTPUT_VARIABLE PROJECT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

if("${PROJECT_VERSION}" STREQUAL "")
    set(${PROJECT_VERSION} v0.0.0-unknown)

    message(
        WARNING
            "GARAGE: Failed to determine version from Git tags. Using default version \"${FOO_VERSION}\"."
    )
endif()

message(STATUS "GARAGE: Version: ${PROJECT_VERSION}")
