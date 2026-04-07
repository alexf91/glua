# Findstb.cmake
# Provides: stb

find_path(STB_INCLUDE_DIR
    NAMES stb_image.h
    PATH_SUFFIXES stb
    PATHS /usr/include /usr/local/include
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(stb REQUIRED_VARS STB_INCLUDE_DIR)

if(stb_FOUND AND NOT TARGET stb)
    add_library(stb INTERFACE IMPORTED)
    set_target_properties(stb PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${STB_INCLUDE_DIR}"
    )
endif()
