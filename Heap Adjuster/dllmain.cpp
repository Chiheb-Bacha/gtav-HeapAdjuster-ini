#include <dllmain.h>
#include <iostream>
#include <fstream>
#include "Globals.h"

// working around a bug in Hooking.Patterns x64 version
template<typename T = void>
auto get_pattern(const char* pattern_string, ptrdiff_t offset = 0)
{
	return hook::pattern(pattern_string).get_first<T>(offset);
}

int heapSize = GetPrivateProfileInt("HEAP_SETTINGS", "HEAP_SIZE", 1024, ".\\HeapAdjuster.ini");
int vanillaHeapSize;

using namespace std;
void Logging()
{
	ofstream myfile;
	myfile.open("HeapAdjuster.log");
	myfile << "Vanilla Heap Size is: " << std::to_string(vanillaHeapSize);
	myfile << "\nHeap Size Adjusted to: " << std::to_string(heapSize);
	myfile.close();
}

void InitializeMod()
{
	int* loc;
	if (g_isEnhanced)
	{
		loc = hook::get_pattern<int32_t>("ba ? ? ? ? 45 31 c0 41 b1 01 e8 ? ? ? ? 48 8d 0d", 1);
	}
	else
	{
		loc = hook::get_pattern<int32_t>("83 C8 01 48 8D 0D ? ? ? ? 41 B1 01 45 33 C0", 17);
	}

	vanillaHeapSize = *loc / 1024 / 1024;

	DWORD oldProtect;
	VirtualProtect(loc, 4, PAGE_EXECUTE_READWRITE, &oldProtect);

	*loc = heapSize * 1024 * 1024;
	// default in 889.22 Enhanced was 648MiB, 
	// default in v3586.0 Legacy was 478MiB

	VirtualProtect(loc, 4, oldProtect, &oldProtect);

	Logging();
}

BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
	if (_Reason == DLL_PROCESS_ATTACH)
	{
		InitializeMod();
	}
	return TRUE;
}