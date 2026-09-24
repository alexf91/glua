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
include(CMakePackageConfigHelpers)

set(GLUA_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/glua")
set(GLUA_TOOLS_DIR "${CMAKE_INSTALL_DATADIR}/glua/tools")

# Install the code generator required by consumers.
install(
    PROGRAMS tools/glua-generate.sh
    DESTINATION ${GLUA_TOOLS_DIR}
)
install(
    DIRECTORY tools/glua/
    DESTINATION ${GLUA_TOOLS_DIR}/glua
)

# Install the CMake package: config, version, module and dependency finders.
configure_package_config_file(
    cmake/glua-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/glua-config.cmake
    INSTALL_DESTINATION ${GLUA_CMAKE_INSTALL_DIR}
    PATH_VARS GLUA_TOOLS_DIR
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/glua-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/glua-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/glua-config-version.cmake
        cmake/glua.cmake
        cmake/Findlua.cmake
        cmake/Findstb.cmake
    DESTINATION ${GLUA_CMAKE_INSTALL_DIR}
)
install(
    EXPORT gluaTargets
    NAMESPACE glua::
    DESTINATION ${GLUA_CMAKE_INSTALL_DIR}
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
