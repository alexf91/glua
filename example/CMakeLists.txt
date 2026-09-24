# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)
project(glua-example C)

# General configuration.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Load the installed Glua package.
# This provides both the `glua::glua` library target and the `glua_target()`
# code generation function.
find_package(Glua CONFIG REQUIRED)

# Create the target.
file(GLOB GLUA_SCHEMAS CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.yaml")
glua_target(
    gluatypes
    "${GLUA_SCHEMAS}"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/gluatypes.h"
)
