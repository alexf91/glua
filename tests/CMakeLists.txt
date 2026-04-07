# SPDX-License-Identifier: Apache-2.0

# Generate the glua file for test_glua.
include(Glua)

function(add_test name)
    add_executable(${name} ${name}.c)
    target_compile_options(${name} PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_features(${name} PUBLIC c_std_23)
    set_property(TARGET ${name} PROPERTY C_STANDARD 23)
    target_include_directories(${name} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${name} PUBLIC m unity::framework glua)
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

glua_target(
    gluatypes
    "${CMAKE_CURRENT_LIST_DIR}/gluatypes.yaml"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/glua.h"
)
add_test(test_glua)
add_dependencies(test_glua gluatypes)

if(GLUA_ENABLE_STB)
    glua_target(
        gluatypes_stb
        "${CMAKE_CURRENT_LIST_DIR}/gluatypes_stb.yaml"
        "${CMAKE_CURRENT_BINARY_DIR}/generated/glua_stb.h"
    )
    add_test(test_glua_stb)
    add_dependencies(test_glua_stb gluatypes_stb)
endif()
