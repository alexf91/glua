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

set(UNITY_COMMIT v2.6.1)
FetchContent_Declare(
  unity
  DOWNLOAD_EXTRACT_TIMESTAMP NEW
  URL https://github.com/ThrowTheSwitch/Unity/archive/${UNITY_COMMIT}.tar.gz
)
FetchContent_MakeAvailable(unity)

# Add library, executables and resources.
add_subdirectory(src/glua)
add_subdirectory(tests)
