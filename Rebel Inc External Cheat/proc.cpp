#pragma once

#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>

using namespace std;

DWORD GetProcId(const char* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry{};
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{

			do
			{
				if (!_stricmp((const char*)procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry{};
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_stricmp((const char*)modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	else
	{
		cout << "!!! INVALID_HANDLE_VALUE: " << hSnap << endl;
		return(FALSE);
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, unsigned int offset)
{
	uintptr_t addr = ptr;
	ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
	addr += offset;
	return addr;
}

float FindCurrentReputation(HANDLE hProcess, uintptr_t DataStructureAddress1)
{
	uintptr_t repAddr1 = FindDMAAddy(hProcess, DataStructureAddress1, 0x808);
	uintptr_t repAddr2 = FindDMAAddy(hProcess, DataStructureAddress1, 0x810);
	uintptr_t repAddr3 = FindDMAAddy(hProcess, DataStructureAddress1, 0x814);
	uintptr_t repAddr4 = FindDMAAddy(hProcess, DataStructureAddress1, 0x818);
	uintptr_t repAddr5 = FindDMAAddy(hProcess, DataStructureAddress1, 0x80c);
	uintptr_t repAddr6 = FindDMAAddy(hProcess, DataStructureAddress1, 0x844);

	float rep1, rep2, rep3, rep4, rep5, rep6;
	ReadProcessMemory(hProcess, (BYTE*)repAddr1, &rep1, sizeof(rep1), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)repAddr2, &rep2, sizeof(rep2), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)repAddr3, &rep3, sizeof(rep3), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)repAddr4, &rep4, sizeof(rep4), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)repAddr5, &rep5, sizeof(rep5), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)repAddr6, &rep6, sizeof(rep6), nullptr);

	float sum = rep1 + rep2 + rep3 + rep4 + rep5 + rep6;
	return sum;
}