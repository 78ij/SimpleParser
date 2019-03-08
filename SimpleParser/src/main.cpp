#include "include/preprocessor.h"
#include "include/list.h"
#include "include/parser.h"

using simple::preproc;
using simple::lexer;
using simple::tokentype;
using simple::token;
using simple::list;
using simple::parser;

void printhelp() {
	cout << "C-like Language code formatter\n";
	cout << "Usage: \n";
	cout << "cf [--version] [--help] <filename> [options]\n";
	cout << "Options:\n";
	cout << "-l,--lexer, print all the tokens in both file order and ascending order.\n";
	cout << "-f,--format <filepath> , format the code. Display the source and formatted code, and output formatted code to specified directory.\n";
	cout << "-a,--ast, display the ast of the code.\n";
	cout << "--version, display the version.\n";
	cout << "--help, print this message.\n";
}
void printversion() {
	cout << "C-like Language code formatter V0.0001 by 78ij\n";
}

int main(int argc,char **argv) {
	if (argc == 1) printhelp();
	else if (string(argv[1]) == "--help") printhelp();
	else if (string(argv[1]) == "--version") printversion();
	else {
		string filename;
		string fp;
		bool output = false;
		filename = string(argv[1]);
		parser pa(filename);
		bool l = false, f = false, a = false;
		for (int i = 2; i < argc; i++) {
			string tmp = string(argv[i]);
			if (tmp == "-l" || tmp == "--lexer") {
				l = true;
			}
			if (tmp == "-f" || tmp == "--format") {
				f = true;
				if (i + 1 < argc) {
					fp = string(argv[i+1]);
					if (fp[0] != '-') {
						output = true;
					}
				}
			}
			if (tmp == "-a" || tmp == "--ast") {
				a = true;
			}
		}
		if (l) {
			list<token> toklist;
			lexer lex(filename);
			token tok = lex.gettoken();
			int row = 0;
			toklist.append(tok);
			cout << "Tokens:-----------------------------------------\n";
			while (tok.type != tokentype::END) {
				if (row != lex.row) {
					row = lex.row;
					cout << row << ": " << endl;
				}
				tok.print();
				tok = lex.gettoken();
				toklist.append(tok);
			}
			int size = toklist.length;
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size - i - 1; j++) {
					if (toklist[j].type > toklist[j + 1].type) {
						token tmp;
						tmp = toklist[j];
						toklist[j] = toklist[j + 1];
						toklist[j + 1] = tmp;
					}
				}
			}
			cout << endl;
			cout << "Tokens in ascending order: " << endl;
			for (int i = 0; i < size; i++) {
				toklist[i].print();
			}
			cout << "Token Count: " << size << endl;
			cout << "\n\n";
			lex.closefile();
		}
		if(a || f)pa.parse();
		if (a) {

			cout << "AST:----------------------------------------------------" << endl;

			pa.print();
			cout << "--------------------------------------------------------\n" << endl;
		}
		if (f) {
			pa.format("");
			if (output == true) {
				pa.format(fp);
			}
		}
		//system("pause");
	}
}