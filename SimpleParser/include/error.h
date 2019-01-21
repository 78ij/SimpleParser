#ifndef SIMP_ERR_H
#define SIMP_ERR_H

#include <string>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

namespace simple {
	class error {
	public :
		error() = delete;
		error(int _row, int _col, string info,string src) : 
			row(_row), col(_col),errorinfo(info),errorsrc(src) {};
		void print()
		{
			cout << "ERROR: " << errorinfo << " at row " << row << " and col " << col << endl;
			cout << errorsrc << endl;
			for (int i = 0; i < col - 1; i++) {
				cout << "~";
			}
			cout << "^" << endl;
		};
	private:
		int row;
		int col;
		string errorinfo;
		string errorsrc;
	};

}

#endif 
