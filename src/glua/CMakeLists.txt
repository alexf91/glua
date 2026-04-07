# SPDX-License-Identifier: Apache-2.0

file(GLOB LIBRARY_SRC CONFIGURE_DEPENDS "*.c")
add_library(glua STATIC ${LIBRARY_SRC})
target_compile_options(glua PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(glua PUBLIC c_std_23)
set_property(TARGET glua PROPERTY C_STANDARD 23)
target_include_directories(glua PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(glua PUBLIC lua::lua)

if(GLUA_ENABLE_STB)
    target_compile_definitions(glua PUBLIC GLUA_ENABLE_STB)
    target_link_libraries(glua PUBLIC stb::stb)
endif()
