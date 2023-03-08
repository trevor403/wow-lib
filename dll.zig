const std = @import("std");
const win = std.os.windows;

const WINAPI = win.WINAPI;
const HINSTANCE = win.HINSTANCE;
const LPVOID = win.LPVOID;
const DWORD = win.DWORD;
const BOOL = win.BOOL;

pub extern fn DllMain(_hinstDLL: HINSTANCE, _fdwReason: DWORD, _lpReserved: LPVOID) callconv(WINAPI) BOOL;

// extra functions
pub usingnamespace @import("extra.zig");
