#
# https://yonamine.me/modernizing-luas-build-system-with-cmake-and-fetchcontent-3694ca21a634
#
cmake_minimum_required(VERSION 3.16)
project(lua LANGUAGES C)

file(GLOB LUA_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB LUA_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# Define executables
set(LUA_EXECUTABLE lua)
set(LUAC_EXECUTABLE luac)

# Separate main files
set(LUA_MAIN src/lua.c)
set(LUAC_MAIN src/luac.c)

list(REMOVE_ITEM LUA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR$}/{LUA_MAIN}" "${CMAKE_CURRENT_SOURCE_DIR}/${LUAC_MAIN}")

# Static and shared libraries
if (NOT TARGET lua_static)
  add_library(lua_static STATIC ${LUA_SOURCES} ${LUA_HEADERS})
  target_compile_features(lua_static PRIVATE c_std_99)
  target_include_directories(lua_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
endif ()

if (NOT TARGET lua_shared)
  add_library(lua_shared SHARED ${LUA_SOURCES} ${LUA_HEADERS})
  target_compile_features(lua_shared PRIVATE c_std_99)
  target_include_directories(lua_shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
endif ()

# Executables
if (NOT TARGET ${LUA_EXECUTABLE})
  add_executable(${LUA_EXECUTABLE} ${LUA_MAIN})
  target_link_libraries(${LUA_EXECUTABLE} PRIVATE lua_static m)
endif ()

if (NOT TARGET ${LUAC_EXECUTABLE})
  add_executable(${LUAC_EXECUTABLE} ${LUAC_MAIN})
  target_link_libraries(${LUAC_EXECUTABLE} PRIVATE lua_static m)
endif ()
