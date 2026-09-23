# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)
project(glua VERSION 0.5.0 LANGUAGES C)

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

include(GNUInstallDirs)

# Install the CMake module and code generator required by consumers.
# These are placed relative to each other so that Glua.cmake can locate the
# tools via the `../tools` path relative to its own location.
install(
    FILES cmake/Glua.cmake
    DESTINATION ${CMAKE_INSTALL_DATADIR}/glua/cmake
)
install(
    PROGRAMS tools/glua-generate.sh
    DESTINATION ${CMAKE_INSTALL_DATADIR}/glua/tools
)
install(
    DIRECTORY tools/glua/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/glua/tools/glua
)

# Generate and install the pkg-config file.
set(GLUA_PC_REQUIRES "lua")
set(GLUA_PC_CFLAGS "")
if (GLUA_ENABLE_STB)
    string(APPEND GLUA_PC_REQUIRES " stb")
    set(GLUA_PC_CFLAGS " -DGLUA_ENABLE_STB")
endif()
configure_file(glua.pc.in glua.pc @ONLY)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/glua.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
