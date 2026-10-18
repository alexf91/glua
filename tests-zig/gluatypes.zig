// SPDX-License-Identifier: Apache-2.0

const std = @import("std");
const zlua = @import("zlua");

pub const BaseType = struct {
    valueInteger: zlua.Integer,
    valueNumber: zlua.Number,
    valueString: []const u8,
};

pub const OptionalDefaultType = struct {
    valueInteger: ?zlua.Integer = 42,
    valueNumber: ?zlua.Number = 42.42,
    valueString: ?[]const u8 = "ABC",
};

pub const OptionalType = struct {
    valueInteger: ?zlua.Integer,
    valueNumber: ?zlua.Number,
    valueString: ?[]const u8,
};

pub const DefaultType = struct {
    valueInteger: zlua.Integer = 42,
    valueNumber: zlua.Number = 42.42,
    valueString: []const u8 = "ABC",
};

pub const ArrayType = struct {
    valueInteger: []zlua.Integer,
    valueNumber: []zlua.Number,
    valueString: [][]const u8,
};
