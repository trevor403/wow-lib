#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

#include "globals.h"
#include "lua.h"

#include "hooks.h"
#include "patch.h"

#include "zig.h"
#include "wowtypes.h"

#define constptr(x) (__builtin_constant_p((void*)x) ? ((void*)x) : ((void*)x))
#define MAKE_HOOK(x) {#x, FP_##x, (void*)&Patch_##x, (void**)&Orig_##x}

// win_input_associate
constexpr void* FP_win_input_associate = constptr(0x86d0a0);
typedef void (*T_win_input_associate)(void);
T_win_input_associate Orig_win_input_associate;
void Patch_win_input_associate(void);

// lua_EULAAccepted
constexpr void* FP_lua_EULAAccepted = constptr(0x4dd740);
typedef int32_t (*T_lua_EULAAccepted)(void*);
T_lua_EULAAccepted Orig_lua_EULAAccepted;
int32_t Patch_lua_EULAAccepted(void* L);

// SFile__OpenEx
constexpr void* FP_SFile__OpenEx = constptr(0x424b50);
typedef int32_t (*T_SFile__OpenEx)(void*, const char*, uint32_t, void**);
T_SFile__OpenEx Orig_SFile__OpenEx;
int32_t Patch_SFile__OpenEx(void* archive, const char* filename, uint32_t flags, void** file);

// SFile__Close
constexpr void* FP_SFile__Close = constptr(0x422910);
typedef int32_t (*T_SFile__Close)(SFile* file);
T_SFile__Close Orig_SFile__Close;
int32_t Patch_SFile__Close(SFile* file);

// SFile__GetFileSize
constexpr void* FP_SFile__GetFileSize = constptr(0x4218c0);
typedef size_t (*T_SFile__GetFileSize)(SFile* file, size_t* filesizeHigh);
T_SFile__GetFileSize Orig_SFile__GetFileSize;
size_t Patch_SFile__GetFileSize(SFile* file, size_t* filesizeHigh);

// SFile__Read
constexpr void* FP_SFile__Read = constptr(0x422530);
typedef int32_t (*T_SFile__Read)(SFile* file, void* buffer, size_t bytestoread, size_t* bytesread, SOVERLAPPED* overlapped, TASYNCPARAMBLOCK* asyncparam);
T_SFile__Read Orig_SFile__Read;
int32_t Patch_SFile__Read(SFile* file, void* buffer, size_t bytestoread, size_t* bytesread, SOVERLAPPED* overlapped, TASYNCPARAMBLOCK* asyncparam);

// Hooks
HOOK_ARRAY HookArray[] =
{
    MAKE_HOOK(win_input_associate),
    MAKE_HOOK(lua_EULAAccepted),
    MAKE_HOOK(SFile__OpenEx),
    MAKE_HOOK(SFile__Close),
    MAKE_HOOK(SFile__GetFileSize),
    MAKE_HOOK(SFile__Read),
};

int HookEntries = sizeof(HookArray) / sizeof(HOOK_ARRAY);

void Patch_win_input_associate(void) {
    setup_dx_hook();
    Orig_win_input_associate();
};

int32_t Patch_lua_EULAAccepted(void* L) {
    printf("CALL Patch_lua_EULAAccepted\n");

    FrameScript__Execute(login_script.c_str(), "CustomLogin.lua", 0);

    return Orig_lua_EULAAccepted(L);
}

const wchar_t *path_base(const char* filename) {
    const size_t size = std::strlen(filename);
    std::wstring wfilename;
    if (size > 0) {
        wfilename.resize(size);
        std::mbstowcs(&wfilename[0], filename, size);
    }

    auto base = fs::path(wfilename).filename();
    return base.c_str();
}

int32_t Patch_SFile__OpenEx(void* archive, const char* filename, uint32_t flags, void** file) {
    // const wchar_t *base = path_base(filename);
    // wprintf(L"CALL Patch_SFile__OpenEx [file](%S)\n", base);

    // ext_btree_put(1000);
    // printf("VAL = %d\n", ext_btree_get(900));
    // printf("VAL = %d\n", ext_btree_get(1000));

    return Orig_SFile__OpenEx(archive, filename, flags, file);
}

int32_t Patch_SFile__Close(SFile* file) {
    // printf("CALL Patch_SFile__Close\n");
    return Orig_SFile__Close(file);
}

size_t Patch_SFile__GetFileSize(SFile* file, size_t* filesizeHigh) {
    // printf("CALL Patch_SFile__GetFileSize\n");
    return Orig_SFile__GetFileSize(file, filesizeHigh);
}

int32_t Patch_SFile__Read(SFile* file, void* buffer, size_t bytestoread, size_t* bytesread, SOVERLAPPED* overlapped, TASYNCPARAMBLOCK* asyncparam) {
    // printf("CALL Patch_SFile__Read\n");
    return Orig_SFile__Read(file, buffer, bytestoread, bytesread, overlapped, asyncparam);
}
