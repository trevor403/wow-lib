#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

bool setup_hooks();
bool setup_dx_hook();
bool cleanup_hooks();

#ifdef __cplusplus
}
#endif
