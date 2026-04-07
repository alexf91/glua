# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)
project(glua C)

# General configuration.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)

# Download dependencies and setup targets if necessary.
set(STB_COMMIT master)
FetchContent_Declare(
  stb
  DOWNLOAD_EXTRACT_TIMESTAMP NEW
  URL https://github.com/nothings/stb/archive/${STB_COMMIT}.tar.gz
)
FetchContent_MakeAvailable(stb)
add_library(stb INTERFACE)
target_include_directories(stb SYSTEM INTERFACE ${stb_SOURCE_DIR})

# Add library and optionall tests.
add_subdirectory(src/glua)
option(WITH_TESTS "Build with tests" OFF)
if (WITH_TESTS)
  if (NOT TARGET unity::framework)
    message("Unity not provided. Looking for package...")
    find_package(unity REQUIRED)
  endif()
  add_subdirectory(tests)
endif()
