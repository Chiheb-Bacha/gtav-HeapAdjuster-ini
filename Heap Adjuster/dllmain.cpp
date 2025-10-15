#include "stdafx.h"

#pragma unmanaged
BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		initializeGame();
		break;
	}
	return TRUE;
}

