cmake_minimum_required(VERSION 3.20)
project(cesil VERSION 0.9.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/core)
add_subdirectory(src/cli)

option(CESIL_BUILD_IDE "Build Qt desktop IDE" ON)
if(CESIL_BUILD_IDE)
    find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Test)
    qt_standard_project_setup()
    add_subdirectory(src/ide)
endif()

enable_testing()
add_subdirectory(tests/core)
if(CESIL_BUILD_IDE)
    add_subdirectory(tests/ide)
endif()
