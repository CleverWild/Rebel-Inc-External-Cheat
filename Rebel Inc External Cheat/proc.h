#pragma once

#include <vector>
#include <windows.h>
#include <TlHelp32.h>

using namespace std;

DWORD GetProcId(const char* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, vector<unsigned int> offsets); 