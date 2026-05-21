# SPDX-License-Identifier: Apache-2.0

set(GLUA_TOOLS "${CMAKE_CURRENT_LIST_DIR}/../tools" CACHE STRING "Path to the tools directory" FORCE)

# Create a target for a Glua header file.
# * <inputs> is semicolon separated list of YAML schema files
# * <output> is the generated header file
function(glua_target target inputs output)
    message("Glua tools directory: ${GLUA_TOOLS}")
    set(GLUA_GENERATE "${GLUA_TOOLS}/glua-generate.sh")
    file(GLOB GLUA_MODULE_FILES CONFIGURE_DEPENDS
        "${GLUA_TOOLS}/glua/*.py"
        "${GLUA_TOOLS}/glua/*.mako"
    )
    get_filename_component(outdir "${output}" DIRECTORY)
    add_custom_command(
        OUTPUT "${output}"
        DEPENDS ${inputs} ${GLUA_GENERATE} ${GLUA_MODULE_FILES}
        COMMAND mkdir -p "${outdir}"
        COMMAND "${GLUA_GENERATE}" ${inputs} | clang-format > "${output}"
        COMMENT "Generating Glua functions"
        VERBATIM
    )
    add_custom_target(${target} ALL DEPENDS "${output}")
endfunction()
