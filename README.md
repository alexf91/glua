# Glua

Glua is a glue code generator for C and Lua.
It is powered by [Mako](https://www.makotemplates.org/).

## Usage

Define an interface in a YAML file.
See `tests/gluatypes.yaml` as an example.

The C header is then generated with `tools/glua-generate.sh`.
See `tests/CMakeLists.txt` as an example.

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

## Usage
