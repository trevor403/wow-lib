const std = @import("std");
const builtin = @import("builtin");

pub fn build(b: *std.build.Builder) !void {
    const target = std.zig.CrossTarget{
        .cpu_arch = .x86,
        .cpu_model = .{
            .explicit = &std.Target.x86.cpu.haswell, // i686, generic, pentium4, haswell
        },
        .os_tag = .windows,
        .abi = .gnu,
    };
    const exe = b.addSharedLibrary(.{
        .name = "custom",
        .target = target,
        .root_source_file = .{ .path = "dll.zig" },
        .optimize = .ReleaseFast,
    });

    const cflags = &[_][]const u8{
        "-std=c2x",
        "-Wall",
        "-g",
    };

    const cxxflags = &[_][]const u8{
        "-std=gnu++2b",
        "-Wall",
        "-g",
    };

    exe.addIncludePath("lib");
    exe.addObjectFile("lib/libwow.a");

    exe.addIncludePath("deps/minhook/include");
    exe.addCSourceFile("deps/minhook/src/buffer.c", cflags);
    exe.addCSourceFile("deps/minhook/src/trampoline.c", cflags);
    exe.addCSourceFile("deps/minhook/src/hde/hde32.c", cflags);
    exe.addCSourceFile("deps/minhook/src/hook.c", cflags);

    exe.addCSourceFile("custom/dll.c", cflags);
    exe.addCSourceFile("custom/skip.c", cflags);
    exe.addCSourceFile("custom/hooks.c", cflags);
    exe.addCSourceFile("custom/patch.cpp", cxxflags);
    exe.addCSourceFile("custom/dx_patch.cpp", cxxflags);
    exe.addCSourceFile("custom/globals.cpp", cxxflags);

    exe.linkLibC();
    exe.linkLibCpp();
    exe.linkSystemLibrary("kernel32");
    exe.linkSystemLibrary("user32");

    // exe.setVerboseCC(true);
    exe.setVerboseLink(true);

    exe.install();
}
