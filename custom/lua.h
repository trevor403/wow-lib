#include <stdint.h>

#define MAKE_DECL(NAME, ADDR, RET, ARGS) \
    typedef RET (__cdecl* T_##NAME) ARGS; \
    static T_##NAME NAME = (T_##NAME)ADDR;

MAKE_DECL(FrameScript__Execute, 0x819210, void, (const char* luastring, const char *filename, int unk0))
MAKE_DECL(FrameScript__RegisterFunction, 0x817f90, void, (const char* name, uint32_t (__cdecl* function)(void*)))
MAKE_DECL(FrameScript__PushString, 0x84e350, const char *, (void* L, const char* s))

MAKE_DECL(lua_tolstring, 0x84e0e0, const char*, (void* L, int index, int length))
MAKE_DECL(lua_isstring, 0x84df60, uint32_t, (void* L, int32_t))

// TODO: lua_gettop

__cdecl uint32_t lua_GetEnvString(void *L);
