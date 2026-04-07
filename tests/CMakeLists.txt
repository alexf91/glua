# SPDX-License-Identifier: Apache-2.0

# Generate the glua file for test_glua.
set(GLUA_GENERATE "${CMAKE_SOURCE_DIR}/tools/glua-generate.sh")
file(GLOB GLUA_MODULE_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/tools/glua/*.py"
    "${CMAKE_SOURCE_DIR}/tools/glua/*.mako"
)
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/generated/glua.h"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/gluatypes.yaml" ${GLUA_GENERATE} ${GLUA_MODULE_FILES}
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/generated/"
    COMMAND
        "${GLUA_GENERATE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/gluatypes.yaml" | clang-format >
                "${CMAKE_CURRENT_BINARY_DIR}/generated/glua.h"
    COMMENT "Generating Glua functions"
    VERBATIM
)
add_custom_target(gluatypes ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/generated/glua.h")

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
