// Rebel Inc External Cheat.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#pragma once
#include <windows.h>

//Data types
#include <vector>

//Console
#include <string>
#include <iostream>
#include <conio.h>
#include <map>

//User libs
#include "proc.h"

using namespace std;

const unsigned char MAXCONSOLEFIELDS = 22;
const string EMPTYSTRING = "                              ";

const char ESCAPE = 27;
const char NUM1 = 49;
const char NUM2 = 50;
const char NUM3 = 51;
const char NUM4 = 52;
const char NUM5 = 53;
const char NUM6 = 54;
const char NUM7 = 55;
const char NUM8 = 56;
const char NUM9 = 57;

const short IDMONEY = 1;
const short IDCORRUPTION = 2;
const short IDINFLATION = 3;
const short IDREPUTATION = 4;

const short IDMENU = 1;
const short IDMEMORY = 11;

class Field {
private:
	string field;
protected:

public:
	Field()
	{
		field = "";
	}

	string GetField()
	{
		return field;
	}

	void SetField(string field)
	{
		this->field = field;
	}
};

class Memory {
private:
	int moneyValue, reputationValue;
	float corruptionValue, inflationValue;

	float roundToDecimal(float num, int decimalPlaces = 1) {
		float powerOfTen = pow(10.0, decimalPlaces);
		return round(num * powerOfTen) / powerOfTen;
	}
public:
	int GetValue(short idValue) {
		switch (idValue) {
		case 1:
			return this->moneyValue;
		case 2:
			return this->corruptionValue;
		case 3:
			return this->inflationValue;
		case 4:
			return this->reputationValue;
		default:
			cout << "INCORRECT ID FOR GET" << endl;
			break;
		}
	}
	void SetValue(short idValue, float value) {
		switch (idValue) {
		case IDMONEY:
			this->moneyValue = (int)value;
			break;
		case IDCORRUPTION:
			this->corruptionValue = roundToDecimal(value);
			break;
		case IDINFLATION:
			this->inflationValue = roundToDecimal(value);
			break;
		case IDREPUTATION:
			this->reputationValue = (int)value;
			break;
		default:
			cout << "INCORRECT ID FOR SET" << endl;
			break;
		}
	}
};

class Console {
private:
	short cursorPos;
	Field Fields[MAXCONSOLEFIELDS];
	HANDLE hStdOut;

	//{ pos, row amount }
	map<short, short> PosMap;

	vector<short> FieldPresetStack;
public:
	Memory memory;

	Console()
	{
		cursorPos = 0;
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

		//Menu sizes
		PosMap[IDMENU] = 5;
		PosMap[IDMEMORY] = 12;
	}

	void GotoXY(short y, short x = 0)
	{
		SetConsoleCursorPosition(hStdOut, { x, y });
	}

	string GetField(short fieldPos)
	{
		return Fields[fieldPos].GetField();
	}

	void SetField(string newField, short fieldPos = -1)
	{
		if (fieldPos == -1) fieldPos = this->cursorPos;

		Fields[fieldPos].SetField(newField);
		this->cursorPos = fieldPos + 1;
	}

	void print()
	{
		for (int i = 0; i < MAXCONSOLEFIELDS; i++)
		{
			GotoXY(i);
			cout << "\033[2K";
		}
		for (int i = 0; i < MAXCONSOLEFIELDS; i++)
		{
			GotoXY(i);
			cout << GetField(i) << endl;
		}
	}

	void SetFieldPreset(string name, short fieldPos = -1)
	{
		if (fieldPos == -1) fieldPos = this->cursorPos + 1;
		if (name == "Menu") {
			this->FieldPresetStack.push_back(IDMENU);
			SetField("==============================");
			SetField("Menu:                         ");
			SetField("1: Memory View                ");
			SetField("2: infinity Money             ");
			SetField("==============================");
		}
		else if (name == "MemoryView")
		{
			this->FieldPresetStack.push_back(IDMEMORY);
			SetField("You have selected memory view ");
			SetField("      To quit press ESC       ");
			SetField("==============================");
			SetField("current money = " + to_string(this->memory.GetValue(IDMONEY)));
			SetField("current corruption = " + to_string(this->memory.GetValue(IDCORRUPTION)));
			SetField("current inflation = " + to_string(this->memory.GetValue(IDINFLATION)));
			SetField("current reputation = " + to_string(this->memory.GetValue(IDREPUTATION)));
			SetField(EMPTYSTRING);
			SetField(EMPTYSTRING);
			SetField(EMPTYSTRING);
			SetField(EMPTYSTRING);
			SetField(EMPTYSTRING);
		}
		else if (name == "Escaping")
		{
			SetField("<- Escaping <-", 0);
			for (int i = 1; i < MAXCONSOLEFIELDS; i++)
			{
				SetField(EMPTYSTRING);
			}
		}
	}

	void ClearPreset(short fieldPos = -1) {
		if (fieldPos == -1) fieldPos = this->cursorPos;
		short currentMenuId = this->FieldPresetStack.back();
		short currentMenuSize = this->PosMap[currentMenuId];
		short baseCursorPos = this->cursorPos - currentMenuSize;
		this->cursorPos = baseCursorPos;
		for (short i = 0; i < currentMenuSize; i++) {
			SetField("");
		}
		this->cursorPos = baseCursorPos;
		this->FieldPresetStack.pop_back();
	}
};

int main()
{
	//Get procID of the target process
	DWORD procId = GetProcId("Rebel Inc. Escalation.exe");
	if (procId == 0)
	{
		cout << "!!procId is null";
		return 1; // "!!procId is null"
	}

	//Getmodulebaseaddress
	uintptr_t moduleBase = GetModuleBaseAddress(procId, "GameAssembly.dll");

	//Get Handle to process
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	//Resolve Data Structure Addresses
	vector<unsigned int> DataStructureOffsets1 = { 0xB8, 0x8, 0x18, 0x78, 0x10, 0x88 };
	uintptr_t DataStructureAddress1 = FindDMAAddy(hProcess, moduleBase + 0x1EED438, DataStructureOffsets1);

	//Resolve Data pointers
	uintptr_t moneyAddr = FindDMAAddy(hProcess, DataStructureAddress1, 0x8f8);
	uintptr_t coruptionAddr = FindDMAAddy(hProcess, DataStructureAddress1, 0x3e8);
	uintptr_t reputationAddr = FindDMAAddy(hProcess, DataStructureAddress1, 0x808); // default reputation, default = 80

	//uintptr_t inflationAddr = FindDMAAddy(hProcess, DataStructureAddress2, 0x3e8);

	//Console part

	SetConsoleTitle("Rebel Inc External Cheat");
	system("CLS");

	Console console;

	char choice;
	while (true)
	{
		console.SetFieldPreset("Menu");
		console.print();

		choice = _getch();
		if (choice == -32) choice = _getch();
		cout << "Code: " << (int)choice << "   " << endl;

		switch (choice)
		{
		case ESCAPE:
			console.SetFieldPreset("Escaping");
			console.print();
			exit(0);
		case NUM1:
			float moneyValue, corruptionValue, inflationValue, reputationValue;
			while (true) {
				reputationValue = FindCurrentReputation(hProcess, DataStructureAddress1);
				ReadProcessMemory(hProcess, (BYTE*)moneyAddr, &moneyValue, sizeof(moneyValue), nullptr);
				ReadProcessMemory(hProcess, (BYTE*)coruptionAddr, &corruptionValue, sizeof(corruptionValue), nullptr);
				//ReadProcessMemory(hProcess, (BYTE*)inflationAddr, &inflationValue, sizeof(inflationValue), nullptr);

				console.memory.SetValue(IDMONEY, moneyValue);
				console.memory.SetValue(IDCORRUPTION, corruptionValue);
				//console.memory.SetValue(IDMONEY, inflationValue);
				console.memory.SetValue(IDREPUTATION, reputationValue);

				console.SetFieldPreset("MemoryView");
				console.print();

				Sleep(200);

				choice = _getch();
				if (choice == -32) choice = _getch();
				if (choice == ESCAPE) {
					console.ClearPreset();
					break;
				}
			}
			break;
		default:
			console.ClearPreset();
			break;
		}

		Sleep(200);
		console.ClearPreset();
	}

	//while (true)
	//{
	//	cout << "=============================" << endl;
	//	cout << "Menu:" << endl;
	//	cout << "option 1" << endl;
	//	cout << "option 2" << endl;
	//	cout << "Choose an option : ";

	//	int choice;
	//	cin >> choice;
	//	clearLastLines(5);

	//	switch (choice) {
	//	case 1:
	//		cout << "=============================" << endl;
	//		cout << "You have selected memory view" << endl;
	//		cout << "To quit press " << QUIT_BUTTON << endl;
	//		done = false;
	//		while (!done)
	//		{
	//			if (_kbhit()) {
	//				char ch = _getch();
	//				if (ch == QUIT_BUTTON) {
	//					break;
	//				}
	//			}

	//			ReadProcessMemory(hProcess, (BYTE*)moneyAddr, &moneyValue, sizeof(moneyValue), nullptr);
	//			ReadProcessMemory(hProcess, (BYTE*)coruptionAddr, &coruptionValue, sizeof(coruptionValue), nullptr);
	//			float currentReputation = FindCurrentReputation(hProcess, DataStructureAddress1);
	//			//ReadProcessMemory(hProcess, (BYTE*)inflationAddr, &inflationValue, sizeof(inflationValue), nullptr);

	//			cout << "===========================" << endl;
	//			cout << "curent money = " << dec << moneyValue << endl;
	//			cout << "curent corruption = " << coruptionValue << endl;
	//			cout << "curent reputation = " << currentReputation << endl;

	//			this_thread::sleep_for(chrono::milliseconds(500));
	//			clearLastLines(4);
	//		}
	//		clearLastLines(4);
	//		break;
	//	default:
	//		cout << "Incorrect answer" << endl;
	//		if (_kbhit()) {
	//			char ch = _getch();
	//			if (ch == QUIT_BUTTON) {
	//				break;
	//			}
	//		}
	//		break;
	//	}

	//	//Write to it
	//	float newMoney = 1337.0;
	//	WriteProcessMemory(hProcess, (BYTE*)moneyAddr, &newMoney, sizeof(newMoney), nullptr);

	//	//Read out
	//	ReadProcessMemory(hProcess, (BYTE*)moneyAddr, &moneyValue, sizeof(moneyValue), nullptr);

	//	//cout << "New money = " << dec << moneyValue << endl;
	//}
	return 0;
}