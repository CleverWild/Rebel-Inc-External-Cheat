#include <vector>
#include <string>
#include <iostream>

using namespace std;

const unsigned char MAXCONSOLEFIELDS = 20;

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

	void SetField(string newfield)
	{
		field = newfield;
	}
};

class Console {
private:
	Field Fields[MAXCONSOLEFIELDS];
public:
	Console() {}

	string GetField(int fieldPos)
	{
		return Fields[fieldPos].GetField();
	}

	void SetField(int fieldPos, string field)
	{
		Fields[fieldPos].SetField(field);
	}
};

int main() {
	Console console;

	cout << console.GetField(0) << endl;

	console.SetField(0, "123 test");

	cout << console.GetField(0) << endl;

	return 0;
}