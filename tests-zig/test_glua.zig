// SPDX-License-Identifier: Apache-2.0

const std = @import("std");
const zlua = @import("zlua");
const glua = @import("gluatypes.zig");

const expect = std.testing.expect;
const Lua = zlua.Lua;

test "lua_init" {
    try expect(true);
}
