#ifndef SIMP_ERR_H
#define SIMP_ERR_H

#include <string>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

namespace simple {
	void color(int);
	class error {
	public :
		error() = delete;
		error(int _row, int _col, string info,string src) : 
			row(_row), col(_col),errorinfo(info),errorsrc(src) {};
		void print();
	private:
		int row;
		int col;
		string errorinfo;
		string errorsrc;
	};

}

#endif 
