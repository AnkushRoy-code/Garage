install(
    TARGETS Garage
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION bin)

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Ankush Roy")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Collection of Ankush's Creation")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

set(CPACK_PACKAGE_CONTACT "Ankush Roy <ankush3411111@gmail.com>")
set (CPACK_PACKAGE_EXECUTABLES "Garage" "Garage" )

# Windows (NSIS) - Options
set(CPACK_NSIS_MUI_ICON "${CMake_SOURCE_DIR}/docs/Logo/AnkushGarage.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMake_SOURCE_DIR}/docs/Logo/AnkushGarage.ico")
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/Garage.exe")
set(CPACK_NSIS_PACKAGE_NAME "Ankush's Garage ${PROJECT_VERSION}")
set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/docs/Logo/AnkushGarage.bmp")
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_CREATE_DESKTOP_LINKS Garage)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Garage ${PROJECT_VERSION}")

# Linux (Debain) - Options
# set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}") # This is the default
# set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "x86-64") # defaults to i386

# Linux (RPM) - Options
# None needed for now

# MacOS (Drag and Drop) - Options
# None needed for now

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
    "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${OS_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}"
)

# set(CPACK_GENERATOR "NSIS;DRAGNDROP;DEB;RPM;ZIP;TGZ")
set(CPACK_GENERATOR "DEB;RPM;ZIP;TGZ")

include(CPack)
