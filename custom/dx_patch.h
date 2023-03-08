#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <d3d9.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef HRESULT (WINAPI* T_DirectX9__EndScene)(LPDIRECT3DDEVICE9);
extern T_DirectX9__EndScene Orig_DirectX9__EndScene;
extern void *PTR_Patch_DirectX9__EndScene;

__cdecl void Callback_EndScene();

#ifdef __cplusplus
}
#endif
