#ifndef SIMP_PREPROC_H
#define SIMP_PREPROC_H

#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include "include/error.h"
#include "include/list.h"


// Due to a stupid thought of mine, two sets of preprocessing are implemented:
// Handled in the preprocessor (behave like a real compiler)
// Or handled in the lexer (Result in the comment being preserved and formatted)
// The latter is compatible with the main purpose of the program (formatting the code)

using std::string;
using std::istringstream;
using std::ostringstream;
using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using simple::error;

namespace simple {
	struct define {
		string def;
		string substitute;
		define() = default;
		define(define &other) {
			def = other.def;
			substitute = other.substitute;
		}
	}; // define function (like #define a(x))not supported. it is far too annoying.
	enum preprocstate {
		NORMAL,COMMENT
	};
	class preproc {
	public:
		bool proc();
		preproc(string _path) : path(_path),state(NORMAL) {};
		preproc() = delete;
	private:
		string path;
		preprocstate state;
		list<define> defines;
		int procinclude();
		bool procdefine();
		bool proccomment();
	};

}

#endif