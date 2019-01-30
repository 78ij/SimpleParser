#include <Windows.h>
#include "include/error.h"

namespace simple {
	void color(int a)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
	}
	void error::print()
	{
		color(12);
		cout << "ERROR: ";
		color(263);
		cout << errorinfo << " at row " << row << " and col " << col << endl;
		cout << errorsrc << endl;
		color(258);
		for (int i = 0; i < col - 1; i++) {
			cout << "~";
		}
		color(263);
		color(268);
		cout << "^" << endl;
		color(263);
	};
}