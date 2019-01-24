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
using std::stoi;
using std::stof;
using std::stold;
using std::string;
using std::isspace;
using std::isalpha;
using std::isxdigit;
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
		CUINT, CINT, CLONG,CULONG, CCHAR, CDOUBLE,CFLOAT,
		LP, RP, LB, RB, LC, RC,
		LT, GT, LE, GE, EQ, OR, AND, ASSIGN,
		SEMI, IF, ELSE,WHILE, FOR, RETURN, BREAK, CONTINUE, EXTERN,UNSIGNED,
		PLUS,MINUS,MULTIPLY,DIVIDE,MOD
	};

	const string tokens[] = {
		"ERROR","END","IDENT","SHORT","LONG","INT","CHAR","FLOAT","DOUBLE",
		"CUINT","CINT","CLONG","CULONG","CCHAR","CDOUBLE","CFLOAT",
		"LP","RP","LB","RB","LC","RC",
		"LT","GT","LE","GE","EQ","OR","AND","ASSIGN",
		"SEMI","IF","ELSE","WHILE","FOR","RETURN","BREAK","CONTINUE","EXTERN","UNSIGNED",
		"PLUS","MINUS","MULTIPLY","DIVIDE","MOD"
	};

	struct token {
		tokentype type;
		string val; // convert using string.stoi, etc
		int icval;
		unsigned int iuval;
		float fval;
		double dval;
		void print() {
			convert();
			cout << "Token Type: " << tokens[type] << endl;
			cout << "String: " << val << "\t";
			switch (type) {
			case CINT:
				cout << "Value: " << icval;
				break;
			case CUINT:
				cout << "Value: " << iuval;
				break;
			case CFLOAT:
				cout << "Value: " << fval;
				break;
			case CDOUBLE:
				cout << "Value: " << dval;
				break;
			}
			cout << endl;
		}
		void convert();
	};

	class lexer {
	public:
		lexer() = delete;
		lexer(string _path) :path(_path), file(_path),row(1){}
		token gettoken();
		int row;
	private:
		token isspsymbol(char c);
		list<string> symbols; // Symbol list
		string path;
		ifstream file;
	};
}

#endif // SIMP_LEXER_H