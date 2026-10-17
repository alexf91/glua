// SPDX-License-Identifier: Apache-2.0

const zlua = @import("zlua");

pub const BaseType = struct {
    valueInteger: zlua.Integer,
    valueNumber: zlua.Number,
    valueString: []u8,
};
