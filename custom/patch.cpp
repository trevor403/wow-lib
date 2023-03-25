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

#define DEF_HOOK(NAME, ADDR, RET, ARGS) \
    constexpr void* FP_##NAME = constptr(ADDR); \
    typedef RET (*T_##NAME) ARGS; \
    T_##NAME Orig_##NAME; \
    RET Patch_##NAME ARGS;

DEF_HOOK(win_input_associate, 0x86d0a0, void, (void))
DEF_HOOK(unk_console_writestr, 0x765270, void, (char *str, uint32_t param_2))
DEF_HOOK(find_text_section_bounds, 0x86b510, int, (void))

DEF_HOOK(FrameScript_Initialize, 0x819bb0, uint32_t, (uint32_t scriptProfileEnabled))
DEF_HOOK(lua_EULAAccepted, 0x4dd740, int32_t, (void* L))

DEF_HOOK(SFile__OpenEx, 0x424b50, int32_t, (void*, const char*, uint32_t, void**))
DEF_HOOK(SFile__Close, 0x422910, int32_t, (SFile* file))
DEF_HOOK(SFile__GetFileSize, 0x4218c0, size_t, (SFile* file, size_t* filesizeHigh))
DEF_HOOK(SFile__Read, 0x422530, int32_t, (SFile* file, void* buffer, size_t bytestoread, size_t* bytesread, SOVERLAPPED* overlapped, TASYNCPARAMBLOCK* asyncparam))

// Hooks
HOOK_ARRAY HookArray[] =
{
    MAKE_HOOK(win_input_associate),
    MAKE_HOOK(unk_console_writestr),
    MAKE_HOOK(find_text_section_bounds),
    MAKE_HOOK(FrameScript_Initialize),
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
}

void Patch_unk_console_writestr(char *str, uint32_t param_2) {
    void *caller = __builtin_return_address(0);
    // char buf[280];
    // strcpy(buf, str);
    // size_t len = wcslen((const wchar_t*)buf);
    // if (buf[len-1] == '\n') buf[len-1] = '\x00';
    printf("[%p] (CONSOLE) %s\n", caller, str);
    Orig_unk_console_writestr(str, param_2);
    return;
}

int Patch_find_text_section_bounds(void) {
    struct SizeOfCode
    {
        DWORD_PTR start;
        DWORD_PTR end;
    };

    // Fix InvalidPtrCheck for callbacks outside of .text section
    SizeOfCode *s = (SizeOfCode*)0xd415b8;
    s->start = 0x00000001;
    s->end = 0x7FFFFFFF;

    return 1;
}

uint32_t Patch_FrameScript_Initialize(uint32_t scriptProfileEnabled) {
    printf("CALL FrameScript_Initialize\n");
    uint32_t ret = Orig_FrameScript_Initialize(scriptProfileEnabled);

    // register more functions
	FrameScript__RegisterFunction("GetEnvString", lua_GetEnvString);

    return ret;
}

int32_t Patch_lua_EULAAccepted(void* L) {
    printf("CALL Patch_lua_EULAAccepted\n");

    lua_queue.enqueue(login_script);

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
    // printf("CALL Patch_SFile__OpenEx [file](%s)\n", filename);

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
