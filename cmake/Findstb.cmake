# Findstb.cmake
# Provides: stb::stb

# Target already exists.
if(TARGET stb::stb)
    return()
endif()

# Legacy stb already exists, so we create an alias.
if(TARGET stb)
    add_library(stb::stb ALIAS stb)
    return()
endif()

find_path(STB_INCLUDE_DIR
    NAMES stb_image.h
    PATH_SUFFIXES stb
    PATHS /usr/include /usr/local/include
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(stb REQUIRED_VARS STB_INCLUDE_DIR)

if(STB_INCLUDE_DIR)
    add_library(stb::stb INTERFACE IMPORTED)
    set_target_properties(stb::stb PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${STB_INCLUDE_DIR}"
    )
    add_library(stb ALIAS stb::stb)
endif()
