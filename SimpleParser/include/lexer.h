#ifndef SIMP_LEXER_H
#define SIMP_LEXER_H

#include <cctype>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <include/list.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::isspace;
using std::isalpha;
using std::isalnum;
using std::isdigit;
using std::ifstream;
using std::stringstream;

namespace simple {
	const string keywords[] = {
		"short", "long", "int", "char","float","double",
		"if","while","for","return","break","continue","extern"
	};

	enum tokentype {
		ERROR, END,
		IDENT,
		SHORT, LONG, INT, CHAR, FLOAT, DOUBLE,
		CUINT, CINT, CLONG,CULONG, CCHAR, CDOUBLE,
		LP, RP, LB, RB, LC, RC,
		LT, GT, LE, GE, EQ, OR, AND, ASSIGN,
		SEMI, IF, ELSE,WHILE, FOR, RETURN, BREAK, CONTINUE, EXTERN,UNSIGNED
	};

	const string tokens[] = {
		"ERROR","END","IDENT","SHORT","LONG","INT","CHAR","FLOAT","DOUBLE",
		"CUINT","CINT","CLONG","CULONG","CCHAR","CDOUBLE",
		"LP","RP","LB","RB","LC","RC",
		"LT","GT","LE","GE","EQ","OR","AND","ASSIGN",
		"SEMI","IF","ELSE","WHILE","FOR","RETURN","BREAK","CONTINUE","EXTERN","UNSIGNED"
	};

	struct token {
		tokentype type;
		string val; // convert using string.stoi, etc
		void print() {
			cout << "TOKENTYPE: " << tokens[type] << endl;
			cout << "VALUE: " << val << endl;
		}
	};

	class lexer {
	public:
		lexer() = delete;
		lexer(string _path) :path(_path), file(_path){}
		token gettoken();
		
	private:

		list<string> symbols; // Symbol list
		string path;
		ifstream file;
	};
}

#endif // SIMP_LEXER_H