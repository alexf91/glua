// SPDX-License-Identifier: Apache-2.0

const std = @import("std");
const zlua = @import("zlua");
const glua = @import("generated/glua.zig");
const gt = @import("gluatypes.zig");

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

fn checkStackLevel(L: *Lua, expected: i32) void {
    std.testing.expectEqual(expected, L.getTop()) catch @panic("Lua stack level mismatch");
}

test "init" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();
}

test "read_integer_type_error_string" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushString("42");
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = glua.Integer.read(ctx.lua, -1);
    try std.testing.expectError(Error.TypeError, value);
}

test "read_integer_type_error_number" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNumber(42.42);
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = glua.Integer.read(ctx.lua, -1);
    try std.testing.expectError(Error.TypeError, value);
}

test "read_integer_nil_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNil();
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = glua.Integer.read(ctx.lua, -1);
    try std.testing.expectError(Error.NilError, value);
}

test "read_integer_stack_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    const valueZero = glua.Integer.read(ctx.lua, 0);
    try std.testing.expectError(Error.StackError, valueZero);

    const valueTop = glua.Integer.read(ctx.lua, ctx.lua.getTop() + 1);
    try std.testing.expectError(Error.StackError, valueTop);
}

test "read_integer_top" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushInteger(42);
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = try glua.Integer.read(ctx.lua, -1);
    try std.testing.expectEqual(42, value);
}

test "read_integer_mid" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushInteger(41);
    _ = ctx.lua.pushInteger(42);
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = try glua.Integer.read(ctx.lua, -2);
    try std.testing.expectEqual(41, value);
}

test "read_number_type_error_string" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushString("42");
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = glua.Number.read(ctx.lua, -1);
    try std.testing.expectError(Error.TypeError, value);
}

test "read_number_nil_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNil();
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = glua.Number.read(ctx.lua, -1);
    try std.testing.expectError(Error.NilError, value);
}

test "read_number_stack_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    const valueZero = glua.Number.read(ctx.lua, 0);
    try std.testing.expectError(Error.StackError, valueZero);

    const valueTop = glua.Number.read(ctx.lua, ctx.lua.getTop() + 1);
    try std.testing.expectError(Error.StackError, valueTop);
}

test "read_number_integer" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushInteger(42);
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = try glua.Number.read(ctx.lua, -1);
    try std.testing.expectEqual(42.0, value);
}

test "read_number_top" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNumber(42.42);
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = try glua.Number.read(ctx.lua, -1);
    try std.testing.expectEqual(42.42, value);
}

test "read_number_mid" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNumber(41.41);
    _ = ctx.lua.pushNumber(42);
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = try glua.Number.read(ctx.lua, -2);
    try std.testing.expectEqual(41.41, value);
}

test "push_string" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    try glua.String.push(ctx.lua, "Hello World!");
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = try ctx.lua.toString(-1);
    try std.testing.expectEqualStrings("Hello World!", value);
}

test "read_string" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushString("Hello World!");
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const top = ctx.lua.getTop();
    const value = try glua.String.read(ctx.lua, -1);
    defer glua.String.free(ctx.lua, value);
    try std.testing.expectEqual(top, ctx.lua.getTop());
    try std.testing.expectEqualStrings("Hello World!", value);
}

test "read_string_nil_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushNil();
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = glua.String.read(ctx.lua, -1);
    try std.testing.expectError(Error.NilError, value);
}

test "read_string_type_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = ctx.lua.pushInteger(42);
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = glua.String.read(ctx.lua, -1);
    try std.testing.expectError(Error.TypeError, value);
}

test "read_integer_array" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = try ctx.lua.doString("return {0, 1, 2, 3}");
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const values = try glua.Integer.readArray(ctx.lua, -1);
    defer glua.Integer.freeArray(ctx.lua, values);
    try std.testing.expectEqual(4, values.len);
    try std.testing.expectEqualSlices(zlua.Integer, &[_]zlua.Integer{ 0, 1, 2, 3 }, values);
}

test "read_integer_array_type_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = try ctx.lua.doString("return {0, 1, 2, '3'}");
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const values = glua.Integer.readArray(ctx.lua, -1);
    try std.testing.expectError(Error.TypeError, values);
}

test "push_read_integer_array" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    try glua.Integer.pushArray(ctx.lua, &[_]zlua.Integer{ 0, 1, 2, 3 });
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const values = try glua.Integer.readArray(ctx.lua, -1);
    defer glua.Integer.freeArray(ctx.lua, values);
    try std.testing.expectEqualSlices(zlua.Integer, &[_]zlua.Integer{ 0, 1, 2, 3 }, values);
}

test "push_read_number_array" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    try glua.Number.pushArray(ctx.lua, &[_]zlua.Number{ 0.1, 1.2, 2.3, 3.4 });
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const values = try glua.Number.readArray(ctx.lua, -1);
    defer glua.Number.freeArray(ctx.lua, values);
    try std.testing.expectEqualSlices(zlua.Number, &[_]zlua.Number{ 0.1, 1.2, 2.3, 3.4 }, values);
}

test "push_read_string_array" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    try glua.String.pushArray(ctx.lua, &[_][]const u8{ "ABC", "BCD", "CDE" });
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const values = try glua.String.readArray(ctx.lua, -1);
    defer glua.String.freeArray(ctx.lua, values);
    try std.testing.expectEqual(3, values.len);
    try std.testing.expectEqualStrings("ABC", values[0]);
    try std.testing.expectEqualStrings("BCD", values[1]);
    try std.testing.expectEqualStrings("CDE", values[2]);
}

test "read_BaseType" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = try ctx.lua.doString(
        \\return {
        \\  valueInteger = 42,
        \\  valueNumber = 42.42,
        \\  valueString = 'ABC'
        \\}
    );
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = try glua.BaseType.read(ctx.lua, -1);
    defer glua.BaseType.free(ctx.lua, value);
    try std.testing.expectEqual(42, value.valueInteger);
    try std.testing.expectEqual(42.42, value.valueNumber);
    try std.testing.expectEqualStrings("ABC", value.valueString);
}

test "read_BaseType_type_error" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = try ctx.lua.doString(
        \\return {
        \\  valueInteger = 'A',
        \\  valueNumber = 42.42,
        \\  valueString = 'ABC'
        \\}
    );
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = glua.BaseType.read(ctx.lua, -1);
    try std.testing.expectError(glua.Error.TypeError, value);
}

test "read_ArrayType" {
    var ctx = try TestContext.setup();
    defer ctx.teardown();

    _ = try ctx.lua.doString(
        \\return {
        \\  valueInteger = {42, 43, 44},
        \\  valueNumber = {42.42, 43.43, 44.44},
        \\  valueString = {'A', 'B', 'C'}
        \\}
    );
    defer checkStackLevel(ctx.lua, ctx.lua.getTop());
    const value = try glua.ArrayType.read(ctx.lua, -1);
    defer glua.ArrayType.free(ctx.lua, value);
    try std.testing.expectEqualSlices(zlua.Integer, &[_]zlua.Integer{ 42, 43, 44 }, value.valueInteger);
    try std.testing.expectEqualSlices(zlua.Number, &[_]zlua.Number{ 42.42, 43.43, 44.44 }, value.valueNumber);
    try std.testing.expectEqualStrings("A", value.valueString[0]);
    try std.testing.expectEqualStrings("B", value.valueString[1]);
    try std.testing.expectEqualStrings("C", value.valueString[2]);
}
