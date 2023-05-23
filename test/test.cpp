#include <iostream>
#include <conio.h>

void printAndCheckKey()
{
	while (true)
	{
		std::cout << "Printing message..." << std::endl;

		if (_kbhit())
		{
			int ch = _getch();
			std::cout << "Key pressed: " << ch << std::endl;
			break;
		}
	}
}

int main() {
	printAndCheckKey();
	return 0;
}