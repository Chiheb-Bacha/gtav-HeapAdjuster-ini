#include <windows.h>

bool g_isEnhanced = [] {
    char path[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA(nullptr), path, MAX_PATH);

    const char* filename = strrchr(path, '\\');
    filename = filename ? filename + 1 : path;

    return (_stricmp(filename, "GTA5_Enhanced.exe") == 0);
    }();