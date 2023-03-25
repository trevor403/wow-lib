#include "lua.h"

#include <stdio.h>
#include <stdlib.h>

__cdecl uint32_t lua_GetEnvString(void *L) {
    const char *key = lua_tolstring(L, 1, 0);

    char *val = getenv(key);
    FrameScript__PushString(L, val);
	return 1;
}
