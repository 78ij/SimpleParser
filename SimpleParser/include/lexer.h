#ifndef SIMP_LEXER_H
#define SIMP_LEXER_H

#include <cctype>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <include/list.h>
#include <include/error.h>


using std::cin;
using std::getline;
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
using std::ostream;
using std::ofstream;
using std::tolower;

namespace simple {
	const string keywords[] = {
		"short", "long", "int", "char","float","double",
		"if","while","for","return","break","continue","extern"
	};

	enum tokentype {
		ERROR = 0, END,
		IDENT,
		SHORT, LONG, INT, CHAR, FLOAT, DOUBLE,UINT,ULONG,USHORT,UCHAR,
		CUINT, CINT, CLONG, CULONG, CCHAR, CDOUBLE, CFLOAT, VOID,
		LP, RP, LB, RB, LC, RC, COMMA,
		LT, GT, LE, GE, EQ, NEQ, OR, AND, ASSIGN,NOT, PLUS, MINUS, MULTIPLY, DIVIDE, MOD,
		SEMI, IF, ELSE, WHILE, FOR, RETURN, BREAK, CONTINUE, EXTERN, UNSIGNED,COM,PREP

	};
	// Unsupported: const string, ++,-- and lots of other things.
	const string tokens[] = {
		"ERROR", "END", "IDENT", "SHORT", "LONG", "INT", "CHAR", "FLOAT", "DOUBLE","UINT","ULONG","USHORT","UCHAR",
		"CUINT", "CINT", "CLONG", "CULONG", "CCHAR", "CDOUBLE", "CFLOAT", "VOID",
		"LP", "RP", "LB", "RB", "LC", "RC", "COMMA",
		"LT", "GT", "LE", "GE", "EQ", "NEQ", "OR", "AND", "ASSIGN","NOT","PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MOD",
		"SEMI", "IF", "ELSE", "WHILE", "FOR", "RETURN", "BREAK", "CONTINUE", "EXTERN", "UNSIGNED","COMMENT","PREPROC",
		//Hardcoded junks
		"<",">","<=",">=","==","!=","||","&&","=","!","+","-","*","/","%"
	};

	struct token {
		tokentype type;
		int row, col;
		string val; // convert using string.stoi, etc
		int icval;
		unsigned int iuval;
		float fval;
		double dval;
		void print() {
			convert();
			color(267);
			cout << "Token No. :" << type << "    ";
			color(270);
			cout << "Token Type: " << tokens[type] << "   ";
			color(11);
			cout << "String: " << val << " ";
			color(13);
			switch (type) {
			case CLONG:
			case CINT:
				cout << "Value: " << icval;
				break;
			case CULONG:
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
			color(263);
			cout << " row: " << row;
			cout << " col: " << col;
			cout << endl;
		}
		void convert();
	};

	class lexer {
	public:
		lexer() = delete;
		lexer(string _path) :path(_path), file(_path),row(1),col(0){
			string tmp;
			while (getline(file,tmp,'\n')) {
				source.append(tmp);
			}
			file.clear();
			file.seekg(-1,std::ios_base::end);
			char c;
			file.get(c);
			file.close();
			ofstream o;
			o.open(_path, std::ios::app);
			if(c != '\n')
				o << "\n";
			o.close();
			file = ifstream(_path);
		}
		token gettoken();
		list<string> source; // Source
		int row, col;
	private:
		token isspsymbol(char c);
		string path;
		ifstream file;
	};
}

#endif // SIMP_LEXER_H