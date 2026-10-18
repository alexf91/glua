// SPDX-License-Identifier: Apache-2.0

const zlua = @import("zlua");

pub const BaseType = struct {
    valueInteger: zlua.Integer,
    valueNumber: zlua.Number,
    valueString: []const u8,
};

pub const ArrayType = struct {
    valueInteger: []zlua.Integer,
    valueNumber: []zlua.Number,
    valueString: [][]const u8,
};
