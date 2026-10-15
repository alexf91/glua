#!/bin/sh

../tools/glua-generate --language zig gluatypes.yaml | zig fmt --stdin > gluatypes.zig
zig test test_glua.zig
