# FindLua.cmake
# Provides: lua::lua

# Prefer already-defined targets
if(TARGET lua::lua)
    return()
endif()

if(TARGET lua)
    add_library(lua::lua ALIAS lua)
    return()
endif()

# Find headers and library (no version suffixes)
find_path(LUA_INCLUDE_DIR
    NAMES lua.h
    PATHS /usr/include /usr/local/include
)

find_library(LUA_LIBRARY
    NAMES lua
    PATHS /usr/lib /usr/local/lib
)

# Validate
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(lua
    REQUIRED_VARS LUA_INCLUDE_DIR LUA_LIBRARY
)

# Create target
if(LUA_INCLUDE_DIR AND LUA_LIBRARY)
    add_library(lua::lua UNKNOWN IMPORTED)
    set_target_properties(lua::lua PROPERTIES
        IMPORTED_LOCATION "${LUA_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
    )
    # Compatibility alias
    add_library(lua ALIAS lua::lua)
endif()
