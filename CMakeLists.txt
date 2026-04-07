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

find_package(lua REQUIRED)

option(GLUA_ENABLE_STB "Build with STB functions" ON)
if (GLUA_ENABLE_STB)
    find_package(stb REQUIRED)
endif()

add_subdirectory(src/glua)

option(GLUA_ENABLE_TESTS "Build with tests" OFF)
if (GLUA_ENABLE_TESTS)
    if (NOT TARGET unity::framework)
        find_package(unity REQUIRED)
    endif()
    add_subdirectory(tests)
endif()
