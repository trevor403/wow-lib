#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <libgen.h> // mingw only

#include "globals.h"

std::string login_script;

SafeQueue<std::string> lua_queue;

void setup_globals() {
    // get the current program
    HINSTANCE program = GetModuleHandle(NULL);

    // full path to current program
    char programPath[MAX_PATH];
    GetModuleFileNameA(program, programPath, sizeof(programPath));

    // directory containing program
    char programDirBuf[MAX_PATH];
    strcpy_s(programDirBuf, sizeof(programDirBuf), programPath);
    char* programDir = dirname(programDirBuf);

    // convert to wchar_t
    const size_t size = std::strlen(programDir);
    std::wstring path;
    if (size > 0) {
        path.resize(size);
        std::mbstowcs(&path[0], programDir, size);
    }

    auto filepath = fs::path(path) / L"Custom" / L"login.lua";
    if (fs::exists(filepath)) {

        std::ifstream input(filepath);
        std::stringstream sstr;

        while(input >> sstr.rdbuf());
        login_script.assign(sstr.str());
    }
}
