#include "skip.h"

// NOTE: this file helps us check the current program and exit to another program

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <libgen.h> // mingw only

#define PATCHED_EXE "Wow-patched.exe"
#define CONCAT(A, B) (A B)
#define EXEC_BLOCKING 1

__cdecl void replacement_main() {
#ifdef EXEC_BLOCKING
    spawnl(_P_WAIT, PATCHED_EXE, PATCHED_EXE, 0);
#else
    execl(PATCHED_EXE, PATCHED_EXE, 0);
#endif
    exit(0);
} 

void change_main(void (*const func_ptr)(void)) {
    void *main_jmp = (void*)0x0406d93;

    DWORD old;
	VirtualProtect(main_jmp, 5, PAGE_EXECUTE_READWRITE, &old);
	*(uint8_t*)main_jmp = 0xe9; // jmp
	*(uint32_t*)(main_jmp + 1) = (uint32_t)func_ptr - (uint32_t)main_jmp - 5;
	VirtualProtect(main_jmp, 5, old, NULL);

    return;
}

bool check_patched() {
    // check if this is patched
    HINSTANCE program = GetModuleHandle(NULL);

    // full path to current program
    char programPath[MAX_PATH];
    GetModuleFileNameA(program, programPath, sizeof(programPath));

    // directory containing program
    char programDirBuf[MAX_PATH];
    strcpy_s(programDirBuf, sizeof(programDirBuf), programPath);
    char* programDir = dirname(programDirBuf);

    // check if current program is named Wow.exe
    char* programName = basename(programPath);
    if (strcmp(programName, "Wow.exe") != 0) {
        return FALSE;
    }

    printf("[PATCHING]\n");

    // lets get out of this program!!!
    printf("OLD PROGRAM %s\n", programName);

    char targetPath[MAX_PATH];
    strcpy_s(targetPath, sizeof(targetPath), programDir);
    strcat_s(targetPath, sizeof(targetPath), CONCAT("\\", PATCHED_EXE));
    printf("NEW PROGRAM %s\n", basename(targetPath));

    char buf[4096];
    size_t bytes;

    FILE *sourceFile = fopen(programPath, "rb");
    FILE *targetFile = fopen(targetPath, "wb");

    fread(&buf[0], 1, sizeof(buf), sourceFile);

    void *pe = &buf[0];
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pe;
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((uint8_t*)pe + pDosHeader->e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = &pNtHeader->FileHeader;
    pFileHeader->Characteristics |= IMAGE_FILE_LARGE_ADDRESS_AWARE;
    // printf("AFTER Characteristics: %04x\n", pFileHeader->Characteristics);

    fwrite(buf, 1, sizeof(buf), targetFile);

    // write out the rest
    while ((bytes = fread(buf, 1, sizeof(buf), sourceFile)) > 0)
        fwrite(buf, 1, bytes, targetFile);

    fclose(sourceFile);
    fclose(targetFile);

    change_main(replacement_main);
    return TRUE;
}
