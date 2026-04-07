# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)
project(glua C)

# General configuration.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)

if (NOT TARGET lua::lua)
  message("Lua not provided. Looking for package...")
  find_package(lua REQUIRED)
endif()

if (NOT TARGET stb::stb)
  message("STB not provided. Looking for package...")
  find_package(stb REQUIRED)
endif()

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
