// Rebel Inc External Cheat.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include "proc.h"


int main()
{
    //Get procID of the target process
    DWORD procId = GetProcId("Rebel Inc. Escalation.exe");

    cout << "procId = " << "0x" << dec << procId << endl;

    //Getmodulebaseaddress
    uintptr_t moduleBase = GetModuleBaseAddress(procId, "GameAssembly.dll");

    cout << "moduleBase = " << "0x" << hex << moduleBase << endl;

    //Get Handle to process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    //Resolve Data Structure Addresses
    vector<unsigned int> DataStructureOffsets1 = { 0xB8, 0x8, 0x18, 0x78, 0x10, 0x88 };
    uintptr_t DataStructureAddress1 = FindDMAAddy(hProcess, moduleBase + 0x1EED438, DataStructureOffsets1);


    //Resolve Data pointers
    uintptr_t moneyAddr = FindDMAAddy(hProcess, DataStructureAddress1, 0x8f8);
    uintptr_t coruptionAddr = FindDMAAddy(hProcess, DataStructureAddress1, 0x3e8);
    uintptr_t reputationAddr = FindDMAAddy(hProcess, DataStructureAddress1, 0x3e8); // default reputation, default = 80
    
    //uintptr_t inflationAddr = FindDMAAddy(hProcess, DataStructureAddress2, 0x3e8);

    //Read money value
    float moneyValue = 0.0;
    ReadProcessMemory(hProcess, (BYTE*)moneyAddr, &moneyValue, sizeof(moneyValue), nullptr);
    float currentReputation = FindCurrentReputation(hProcess, DataStructureAddress1);

    cout << "curent money = " << dec << moneyValue << endl;
    cout << "curent reputation = " << currentReputation << endl;

    //Write to it
    float newMoney = 1337.0;
    WriteProcessMemory(hProcess, (BYTE*)moneyAddr, &newMoney, sizeof(newMoney), nullptr);

    //Read out 
    ReadProcessMemory(hProcess, (BYTE*)moneyAddr, &moneyValue, sizeof(moneyValue), nullptr);

    //cout << "New money = " << dec << moneyValue << endl;


    getchar();
    return 0;
}


