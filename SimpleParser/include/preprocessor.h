#ifndef SIMP_PREPROC_H
#define SIMP_PREPROC_H

#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include "include/error.h"
#include "include/list.h"

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