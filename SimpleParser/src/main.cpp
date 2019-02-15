#include "include/preprocessor.h"
#include "include/list.h"
#include "include/parser.h"

using simple::preproc;
using simple::lexer;
using simple::tokentype;
using simple::token;
using simple::list;
using simple::parser;

int main() {
	list<token> toklist;
	/*preproc p("sample.txt");
	if (!(p.proc())) {
		system("pause");
		return 0;
	}*/
	lexer lex("sample.txt");
	token tok = lex.gettoken();
	int row = 0;
	toklist.append(tok);
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
	parser pa("sample.txt");
	pa.parse();
	cout << "--------------------------------------------------------" << endl;

	pa.print();
	cout << "--------------------------------------------------------" << endl;
	pa.format();
	system("pause");
}