#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

typedef void* LPUNKNOWN;

#include <stdio.h>

typedef HRESULT(WINAPI* T_DINPUT8_DirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);

FARPROC originalSymbol;
HINSTANCE originalDLL;

HINSTANCE customDLL;

// Export symbol as DirectInput8Create and proxy to the real DirectInput8Create
HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter) {
    printf("CALL DirectInput8Create\n");
    return ((T_DINPUT8_DirectInput8Create)originalSymbol)(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

BOOL WINAPI DllMain(
    HINSTANCE _hinstDLL,  // handle to DLL module
    DWORD _fdwReason,     // reason for calling function
    LPVOID _lpReserved)   // reserved
{
    switch (_fdwReason) {
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
        // Return FALSE to fail DLL load.
        {
            // Proxy setup

            // DLL has already been loaded, so act like we've already setup the proxy.
            if (originalDLL)
                return TRUE;

            // Get path to System32
            char winpath[MAX_PATH];
            GetSystemDirectoryA(winpath, sizeof(winpath));
            strcat_s(winpath, sizeof(winpath), "\\DINPUT8.dll");

            // Load the real DINPUT8.dll
            originalDLL = LoadLibraryA(winpath);

            // Verify that we have actually loaded the DLL.
            if (!originalDLL)
                return FALSE;

            originalSymbol = GetProcAddress(originalDLL, "DirectInput8Create");

            if (!originalSymbol)
                return FALSE;

            printf("Proxy DINPUT8.dll loaded\n");

            // Custom setup

            // Load the real DINPUT8.dll
            customDLL = LoadLibraryA("custom.dll");

            // Verify that we have actually loaded the DLL.
            if (!customDLL)
                return FALSE;
        }
        break;
    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        FreeLibrary(originalDLL);
        FreeLibrary(customDLL);
        break;
    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;
    case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
        break;
    }
    return TRUE; // Successful.
}
