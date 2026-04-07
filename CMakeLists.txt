# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)
project(glua C)

# General configuration.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(GLUA_MODULE_PATH
    "${CMAKE_CURRENT_LIST_DIR}/cmake"
    CACHE INTERNAL "Path to glua CMake modules"
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)

find_package(lua REQUIRED)
find_package(stb REQUIRED)

# Add library and optional tests.
add_subdirectory(src/glua)
option(WITH_TESTS "Build with tests" OFF)
if (WITH_TESTS)
    if (NOT TARGET unity::framework)
        message("Unity not provided. Looking for package...")
        find_package(unity REQUIRED)
    endif()
    add_subdirectory(tests)
endif()
