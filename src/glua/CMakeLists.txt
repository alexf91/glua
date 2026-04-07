# SPDX-License-Identifier: Apache-2.0

file(GLOB LIBRARY_SRC CONFIGURE_DEPENDS "*.c")
add_library(glua STATIC ${LIBRARY_SRC})
target_compile_options(glua PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(glua PUBLIC c_std_23)
set_property(TARGET glua PROPERTY C_STANDARD 23)
target_include_directories(glua PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(glua PUBLIC lua stb::stb)
