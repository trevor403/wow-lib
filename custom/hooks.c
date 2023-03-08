#include "hooks.h"
#include "patch.h"
#include "dx_patch.h"
#include "MinHook.h"

bool setup_hooks() {
    MH_STATUS status = MH_OK;

    status = MH_Initialize();
    if (status != MH_OK) {
        printf("Failed to MH_Initialize\n");
        return FALSE;
    }

	for(int i = 0; i < HookEntries; i++) {
        printf("Running MH_CreateHook(%s)\n", HookArray[i].name);
        MH_STATUS status = MH_CreateHook(HookArray[i].addr, HookArray[i].proxy, HookArray[i].original);
        if (status != MH_OK) {
            printf("Failed to MH_CreateHook(%s)\n", HookArray[i].name);
            return FALSE;
        }
	}

    status = MH_EnableHook(NULL);
    if (status != MH_OK) {
        printf("Failed to MH_EnableHook\n");
        return FALSE;
    }

    printf("Hooking complete!\n");
    return TRUE;
}

static bool _is_dx_hook_installed = FALSE;
bool setup_dx_hook() {
    if (_is_dx_hook_installed) {
        return TRUE;
    }

    _is_dx_hook_installed = TRUE;

    uint32_t endScenePointer = 0;
    uint32_t resetPointer = 0;

	uint32_t ptr = 0;
	do {
		ptr = *(uint32_t*)0x00C5DF88;
	} while (ptr == 0);
    printf("dx_device = 0x%08x\n", ptr);

	do {
		ptr = *(uint32_t*)(ptr + 0x397C);
	}while (ptr == 0);
    printf("scene_ptr = 0x%08x\n", ptr);

	// Scene	
	do {
		ptr = *(uint32_t*)ptr;
	} while (ptr == 0);
    printf("scene = 0x%08x\n", ptr);

	do {
		endScenePointer = *(uint32_t*)(ptr + 0xA8);   //42 * 4
        
	} while (endScenePointer == 0);

	do {
		resetPointer = *(uint32_t*)(ptr + 0x40);   //16 * 4
	} while (resetPointer == 0);

    printf("endScenePointer = 0x%08x\n", endScenePointer);

    const void* FP_DirectX9_endScene = (void*)endScenePointer;
    MH_STATUS status = MH_CreateHook(FP_DirectX9_endScene, PTR_Patch_DirectX9__EndScene, (void**)&Orig_DirectX9__EndScene);
    if (status != MH_OK) {
        printf("Failed to MH_CreateHook(DirectX9__EndScene)\n");
        return FALSE;
    }

    status = MH_EnableHook(NULL);
    if (status != MH_OK) {
        printf("Failed to MH_EnableHook\n");
        return FALSE;
    }

    printf("DX Hooking complete!\n");
    return TRUE;
}

bool cleanup_hooks() {
    MH_STATUS status = MH_Uninitialize();
    if (status != MH_OK) {
        return FALSE;
    }

    return TRUE;
};
