# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)
project(glua-example C)

# General configuration.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)

# Download Glua.
# In a regular project, this would be the path to this repository.
FetchContent_Declare(
    glua
    GIT_REPOSITORY "file://${CMAKE_CURRENT_LIST_DIR}/.."
    GIT_TAG main
)
FetchContent_MakeAvailable(glua)

list(APPEND CMAKE_MODULE_PATH "${GLUA_MODULE_PATH}")
include(Glua)

# Create the target.
file(GLOB GLUA_SCHEMAS CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.yaml")
glua_target(
    gluatypes
    "${GLUA_SCHEMAS}"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/gluatypes.h"
)
