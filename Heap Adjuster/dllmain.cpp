#include <dllmain.h>
#include <iostream>
#include <fstream>
#include "Globals.h"
#include "pattern.h"
#include <string>

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

	
	BYTE* loc;
	if (true)
	{
		loc = BytePattern((BYTE*)"\xBA\x00\x00\x00\x00\x45\x31\xC0\x41\xB1\x01\xE8\x00\x00\x00\x00\x48\x8D\x0D", "x????xxxxxxx????xxx").get(1);
	}
	else
	{
		loc = BytePattern((BYTE*)"\x83\xC8\x01\x48\x8D\x0D\x00\x00\x00\x00\x41\xB1\x01\x45\x33\xC0", "xxxxxx????xxxxxx").get(17);
	}

	vanillaHeapSize = *(int*)loc / 1024 / 1024;

	DWORD oldProtect;

	VirtualProtect(loc, 4, PAGE_EXECUTE_READWRITE, &oldProtect);

	*(int*)loc = heapSize * 1024 * 1024;
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