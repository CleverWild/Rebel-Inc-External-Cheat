#include <vector>
#include <windows.h>
#include <string>
#include <iostream>
#include <conio.h>
#include <map>

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

class Console {
private:
	short cursorPos;
	Field Fields[MAXCONSOLEFIELDS];
	HANDLE hStdOut;

	//{ pos, row amount }
	map<string, short> PosMap;
public:
	Console()
	{
		cursorPos = -1;
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

		PosMap["Menu"] = 5;
		PosMap["MemoryView"] = 7;
		PosMap["Escaping"] = MAXCONSOLEFIELDS;
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
		if (fieldPos == -1) fieldPos = this->cursorPos + 1;

		Fields[fieldPos].SetField(newField);
		cursorPos = fieldPos;
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
			SetField("==============================");
			SetField("Menu:                         ");
			SetField("1: Memory View                ");
			SetField("2: infinity Money             ");
			SetField("==============================");
		}
		else if (name == "MemoryView")
		{
			SetField("You have selected memory view ");
			SetField("      To quit press ESC       ");
			SetField("==============================");
			SetField("current money = " + (string)moneyValue);
			SetField(EMPTYSTRING);
			SetField(EMPTYSTRING);
			SetField(EMPTYSTRING);
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
};

int main() {
	SetConsoleTitle(L"Rebel Inc External Cheat");
	system("CLS");

	Console console;

	char chouce;
	while (true)
	{
		chouce = _getch();
		if (chouce == -32) chouce = _getch();
		cout << "Code: " << (int)chouce << endl;

		switch (chouce)
		{
		case ESCAPE:
			console.SetFieldPreset("Escaping");
			console.print();
			exit(0);
		case NUM1:

		default: break;
		}

		Sleep(1);
	}

	/*
	while (true)
	{
		console.SetFieldPreset("Main");
		console.print();

		char choice = _getch();
		cout << choice << endl;

		Sleep(200);
	}
	*/

	return 0;
}