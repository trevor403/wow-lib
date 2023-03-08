#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

typedef void* LPUNKNOWN;

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

#include "hooks.h"
#include "patch.h"

// from Zig
#include "zig.h"

static bool process_attach() {
    setlocale(LC_ALL, "en_US.utf8");

    // zig_start_routine(setup_dx_hook);

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
            printf("(Custom) DLL_PROCESS_ATTACH\n");
            DisableThreadLibraryCalls((HMODULE)_hinstDLL);
            if (process_attach() != TRUE)
                return FALSE;
        }
        break;
    case DLL_PROCESS_DETACH:
        {
            printf("(Custom) DLL_PROCESS_DETACH\n");
            if (process_detach() != TRUE)
                return FALSE;
        }
        break;
    }
    return TRUE; // Successful.
}