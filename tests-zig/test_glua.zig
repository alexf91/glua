// SPDX-License-Identifier: Apache-2.0

const std = @import("std");
const zlua = @import("zlua");
const glua = @import("gluatypes.zig");

const Lua = zlua.Lua;
const Error = glua.Error;

/// Test fixture for an allocator and a Lua interpreter.
const TestContext = struct {
    allocator: std.mem.Allocator,
    lua: *Lua,

    fn setup() !TestContext {
        const allocator = std.testing.allocator;
        const lua = try Lua.init(allocator);

        return .{
            .allocator = allocator,
            .lua = lua,
        };
    }

    fn teardown(self: *TestContext) void {
        self.lua.deinit();
    }
};

test "init" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();
}

test "read_integer_type_error_string" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushString("42");
    const value = glua.readInteger(ctx.lua, -1);
    try std.testing.expectError(Error.TypeError, value);
}

test "read_integer_type_error_number" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNumber(42.42);
    const value = glua.readInteger(ctx.lua, -1);
    try std.testing.expectError(Error.TypeError, value);
}

test "read_integer_nil_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNil();
    const value = glua.readInteger(ctx.lua, -1);
    try std.testing.expectError(Error.NilError, value);
}

test "read_integer_stack_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    const valueZero = glua.readInteger(ctx.lua, 0);
    try std.testing.expectError(Error.StackError, valueZero);

    const valueTop = glua.readInteger(ctx.lua, ctx.lua.getTop() + 1);
    try std.testing.expectError(Error.StackError, valueTop);
}

test "read_integer_top" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushInteger(42);
    const value = try glua.readInteger(ctx.lua, -1);
    try std.testing.expectEqual(42, value);
}

test "read_integer_mid" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushInteger(41);
    _ = ctx.lua.pushInteger(42);
    const value = try glua.readInteger(ctx.lua, -2);
    try std.testing.expectEqual(41, value);
}

test "read_number_type_error_string" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushString("42");
    const value = glua.readNumber(ctx.lua, -1);
    try std.testing.expectError(Error.TypeError, value);
}

test "read_number_nil_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNil();
    const value = glua.readNumber(ctx.lua, -1);
    try std.testing.expectError(Error.NilError, value);
}

test "read_number_stack_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    const valueZero = glua.readNumber(ctx.lua, 0);
    try std.testing.expectError(Error.StackError, valueZero);

    const valueTop = glua.readNumber(ctx.lua, ctx.lua.getTop() + 1);
    try std.testing.expectError(Error.StackError, valueTop);
}

test "read_number_integer" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushInteger(42);
    const value = try glua.readNumber(ctx.lua, -1);
    try std.testing.expectEqual(42.0, value);
}

test "read_number_top" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNumber(42.42);
    const value = try glua.readNumber(ctx.lua, -1);
    try std.testing.expectEqual(42.42, value);
}

test "read_number_mid" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNumber(41.41);
    _ = ctx.lua.pushNumber(42);
    const value = try glua.readNumber(ctx.lua, -2);
    try std.testing.expectEqual(41.41, value);
}

test "push_string" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    try glua.pushString(ctx.lua, "Hello World!");
    const value = try ctx.lua.toString(-1);
    try std.testing.expectEqualStrings("Hello World!", value);
}

test "read_string" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushString("Hello World!");
    const top = ctx.lua.getTop();
    const value = try glua.readString(ctx.lua, -1);
    try std.testing.expectEqual(top, ctx.lua.getTop());
    try std.testing.expectEqualStrings("Hello World!", value);
    glua.freeString(ctx.lua, value);
}

test "read_string_nil_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNil();
    const value = glua.readString(ctx.lua, -1);
    try std.testing.expectError(Error.NilError, value);
}

test "read_string_type_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushInteger(42);
    const value = glua.readString(ctx.lua, -1);
    try std.testing.expectError(Error.TypeError, value);
}

test "read_array_integer" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = try ctx.lua.doString("return {0, 1, 2, 3}");
    const values = try glua.readIntegerArray(ctx.lua, -1);
    try std.testing.expectEqual(4, values.len);
    try std.testing.expectEqualSlices(zlua.Integer, &[_]zlua.Integer{ 0, 1, 2, 3 }, values);

    glua.freeIntegerArray(ctx.lua, values);
}

test "read_array_integer_type_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = try ctx.lua.doString("return {0, 1, 2, '3'}");
    const values = glua.readIntegerArray(ctx.lua, -1);
    try std.testing.expectError(Error.TypeError, values);
}
