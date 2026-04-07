# Glua

Glua is a glue code generator for C and Lua.
It is powered by [Mako](https://www.makotemplates.org/).

## Building

The library can be built with CMake:

```bash
mkdir build
cmake -DWITH_TESTS=ON ..
make
```

By default, dependencies already installed on the system are used.
However, since the library is usually imported into another project and statically linked, targets already provided by the parent project are preferred.
The following targets can be provided:

- `lua::lua` for Lua 5.5 (either static or dynamic)
- `stb::stb` for [STB](http://github.com/nothings/stb)

Support for dynamic arrays is enabled by default, but can be deactivated by setting `GLUA_ENABLE_STB=OFF` during configuration.

## Usage

Define an interface in a YAML file.
See `tests/gluatypes.yaml` as an example.

Add it to the project by including `Glua` and generate a target with `glua_target()`.

```cmake
list(APPEND CMAKE_MODULE_PATH "${GLUA_MODULE_PATH}")
include(Glua)

# Create the target.
glua_target(
  gluatypes
  "${CMAKE_CURRENT_SOURCE_DIR}/gluatypes.yaml"
  "${CMAKE_CURRENT_BINARY_DIR}/generated/gluatypes.h"
)
```
