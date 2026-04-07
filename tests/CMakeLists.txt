# SPDX-License-Identifier: Apache-2.0

# Generate the glua file for test_glua.
include(Glua)
glua_target(
    gluatypes
    "${CMAKE_CURRENT_LIST_DIR}/gluatypes.yaml"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/glua.h"
)

# Testrunners
file(GLOB TESTFILES CONFIGURE_DEPENDS "*.c")
foreach(TESTFILE ${TESTFILES})
    get_filename_component(TESTNAME ${TESTFILE} NAME_WLE)
    add_executable(${TESTNAME} ${TESTFILE})
    target_compile_options(${TESTNAME} PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_features(${TESTNAME} PUBLIC c_std_23)
    set_property(TARGET ${TESTNAME} PROPERTY C_STANDARD 23)
    target_include_directories(${TESTNAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${TESTNAME} PUBLIC m unity::framework glua)
    target_include_directories(${TESTNAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    add_dependencies(${TESTNAME} gluatypes)
endforeach()
