// SPDX-License-Identifier: Apache-2.0

const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const zlua_dep = b.dependency("zlua", .{
        .target = target,
        .optimize = optimize,
    });

    const test_mod = b.createModule(.{
        .root_source_file = b.path("tests-zig/test_glua.zig"),
        .target = target,
        .optimize = optimize,
        .imports = &.{.{
            .name = "zlua",
            .module = zlua_dep.module("zlua"),
        }},
    });

    const test_filter = b.option(
        []const u8,
        "test-filter",
        "Filter tests by substring",
    );
    const filters: []const []const u8 =
        if (test_filter) |filter| &.{filter} else &.{};

    const tests = b.addTest(.{
        .root_module = test_mod,
        .filters = filters,
    });

    const run_tests = b.addRunArtifact(tests);
    run_tests.has_side_effects = true;
    const test_step = b.step("test", "Run tests");
    test_step.dependOn(&run_tests.step);
}
