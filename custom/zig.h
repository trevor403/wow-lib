#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

extern __cdecl void zig_start_routine(void (*const a0)(void));
extern __cdecl void zig_sleep_msec(uint32_t ms);

extern __cdecl void ext_btree_put(uint32_t key);
extern __cdecl uint8_t ext_btree_get(uint32_t key);
extern __cdecl void ext_btree_deinit();

#ifdef __cplusplus
}
#endif
