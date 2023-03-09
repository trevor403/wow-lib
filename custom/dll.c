#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

typedef void* LPUNKNOWN;

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

#include "skip.h"
#include "hooks.h"
#include "patch.h"

#include "globals.h"

// from Zig
#include "zig.h"

// from Go
#include "go.h"

static char *program = "Custom";
static bool skipping = FALSE;

static void go_setup() {
    go_hello();
}

static bool process_attach() {
    setlocale(LC_ALL, "en_US.utf8");

    go_init();
    zig_start_routine(go_setup); // new stack

    setup_globals();

    if (!setup_hooks()) return FALSE;
    return TRUE;
}

static bool process_detach() {
    ext_btree_deinit();

    if (!cleanup_hooks()) return FALSE;
    return TRUE;
}

// Return FALSE to fail DLL load.
BOOL WINAPI DllMain(HINSTANCE _hinstDLL, DWORD _fdwReason, LPVOID _lpReserved) {
    switch (_fdwReason) {
	case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls((HMODULE)_hinstDLL);

            skipping = check_patched();
            if (skipping) program = "SKIP WRAPPER";
            printf("(%s) DLL_PROCESS_ATTACH\n", program);

            if (skipping) {
                return TRUE; // early exit
            }

            if (process_attach() != TRUE)
                return FALSE;
        }
        break;
    case DLL_PROCESS_DETACH:
        {
            printf("(%s) DLL_PROCESS_DETACH\n", program);

            if (skipping) {
                return TRUE; // early exit
            }

            if (process_detach() != TRUE)
                return FALSE;
        }
        break;
    }
    return TRUE; // Successful.
}
