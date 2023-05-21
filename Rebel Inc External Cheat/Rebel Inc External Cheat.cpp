// Rebel Inc External Cheat.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

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

    //Resolve base address of the pointer chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x1EED438;

    cout << "dynamicPtrBaseAddr = " << "0x" << hex << dynamicPtrBaseAddr << endl;

    //Resolve money pointer chain
    vector<unsigned int> moneyOffsets = { 0xB8, 0x8, 0x18, 0x78, 0x10, 0x88, 0x8f8 };
    uintptr_t moneyAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, moneyOffsets);

    cout << "moneyAddr = " << "0x" << hex << moneyAddr << endl;

    //Read money value
    float moneyValue = 0.0f;

    ReadProcessMemory(hProcess, (BYTE*)moneyAddr, &moneyValue, sizeof(moneyValue), nullptr);
    cout << "curent money = " << dec << moneyValue << endl;

    //Write to it
    float newMoney = 1337.0f;
    WriteProcessMemory(hProcess, (BYTE*)moneyAddr, &newMoney, sizeof(newMoney), nullptr);

    //Read out 
    ReadProcessMemory(hProcess, (BYTE*)moneyAddr, &moneyValue, sizeof(moneyValue), nullptr);

    cout << "New money = " << dec << moneyValue << endl;


    getchar();
    return 0;
}