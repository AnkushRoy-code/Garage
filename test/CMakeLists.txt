FetchContent_MakeAvailable(Catch2)

add_executable(GarageTests
    Core/Console.cpp
    Utils/Time.cpp
)

target_include_directories(GarageTests PRIVATE ../src)
target_link_libraries(GarageTests PRIVATE Core Utils Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(GarageTests)
