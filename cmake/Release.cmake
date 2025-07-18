# Setup
if(RELEASE_BUILD)
    include(GNUInstallDirs)
    include(InstallRequiredSystemLibraries)

    install(
        TARGETS Garage
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/res
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}
    )

    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_VENDOR "Ankush Roy")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Collection of Ankush's Creation")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
    set(CPACK_PACKAGE_CONTACT "Ankush Roy <ankush3411111@gmail.com>")
    set(CPACK_PACKAGE_EXECUTABLES "Garage" "Garage")

    # Windows (NSIS) - Options
    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/res/Images/Logo/AnkushGarage.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/res/Images/Logo/AnkushGarage.ico")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/Garage.exe")
    set(CPACK_NSIS_PACKAGE_NAME "Ankush's Garage ${PROJECT_VERSION}")

    if(WIN32)
        set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/res/Images/Logo\\\\AnkushGarage.bmp")
    endif()

    if(!WIN32)
        set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/res/Images/Logo/AnkushGarage.bmp")
    endif()

    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_CREATE_DESKTOP_LINKS Garage)
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "Garage ${PROJECT_VERSION}")

    # Linux (Debain) - Options

    # set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}") # This is the default
    # set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "x86-64") # defaults to i386

    # Linux (RPM) - Options

    # None needed for now

    # MacOS (Drag and Drop) - Options

    # Naming the Packed(installer) file
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(OS_NAME "Linux")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(OS_NAME "Windows")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(OS_NAME "macOS")
    else()
        set(OS_NAME "UnknownOS")
    endif()

    set(CPACK_PACKAGE_FILE_NAME
        "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${OS_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}"
    )

    # set(CPACK_GENERATOR "NSIS;DRAGNDROP;DEB;RPM;ZIP;TGZ")
    set(CPACK_GENERATOR "ZIP;TGZ")

    include(CPack)
endif()
