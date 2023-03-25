#include <filesystem>

#include "dx_patch.h"
#include "globals.h"
#include "lua.h"

typedef HRESULT (WINAPI* T_DirectX9__EndScene)(LPDIRECT3DDEVICE9);
T_DirectX9__EndScene Orig_DirectX9__EndScene;
extern "C" HRESULT WINAPI Patch_DirectX9__EndScene(LPDIRECT3DDEVICE9 pDevice) {
    Callback_EndScene();
    return Orig_DirectX9__EndScene(pDevice);
}
void *PTR_Patch_DirectX9__EndScene = (void*)&Patch_DirectX9__EndScene;

// main thread
__cdecl void Callback_EndScene() {
    while (!lua_queue.empty()) {
        auto script = lua_queue.dequeue();
        FrameScript__Execute(script.c_str(), "exec.lua", 0);
    }
    return;
}
