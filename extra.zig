const std = @import("std");
const print = std.debug.print;
const btreemap = @import("deps/zig-btreemap/btreemap.zig");

var tree = btreemap.BTreeMap(u32, u8).init(std.heap.c_allocator);

pub export fn ext_btree_put(key: u32) callconv(.C) void {
    _ = tree.fetchPut(key, 1) catch unreachable;
}

pub export fn ext_btree_get(key: u32) callconv(.C) u8 {
    if (tree.get(key) != null)
        return 1;

    return 0;
}

pub export fn ext_btree_deinit() callconv(.C) void {
    tree.deinit() catch unreachable;
}

pub const thread_func = ?*const fn () callconv(.C) void;
pub var zig_thread_func: thread_func = undefined;
pub var zig_thread_mutex = std.Thread.Mutex{};
fn zig_routine() void {
    zig_thread_func.?();
}

pub export fn zig_start_routine(f: thread_func) callconv(.C) void {
    print("Starting Zig thread...\n", .{});
    zig_thread_mutex.lock();
    zig_thread_func = f;
    const thread = std.Thread.spawn(.{}, zig_routine, .{}) catch unreachable;
    thread.detach();
    zig_thread_mutex.unlock();
}

pub export fn zig_sleep_msec(ms: u32) callconv(.C) void {
    std.time.sleep(ms * std.time.ns_per_ms);
}
