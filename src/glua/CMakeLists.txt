# SPDX-License-Identifier: Apache-2.0

include(GNUInstallDirs)

file(GLOB LIBRARY_SRC CONFIGURE_DEPENDS "*.c")
add_library(glua STATIC ${LIBRARY_SRC})
target_compile_options(glua PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(glua PUBLIC c_std_23)
set_property(TARGET glua PROPERTY C_STANDARD 23)
target_include_directories(glua PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(glua PUBLIC lua)

if(GLUA_ENABLE_STB)
    target_compile_definitions(glua PUBLIC GLUA_ENABLE_STB)
    target_link_libraries(glua PUBLIC stb)
endif()

add_library(glua::glua ALIAS glua)

# Install the library and its public header.
install(TARGETS glua EXPORT gluaTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    FILES ${PROJECT_SOURCE_DIR}/include/glua/glua.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/glua
)
