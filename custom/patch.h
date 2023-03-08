#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Hooks
typedef struct {
    LPCSTR name;
	LPVOID addr;
	LPVOID proxy;
	LPVOID *original;
} HOOK_ARRAY;

extern HOOK_ARRAY HookArray[];
extern int HookEntries;
