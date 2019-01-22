#include "include/preprocessor.h"
#include "include/lexer.h"

using simple::preproc;
using simple::lexer;
using simple::tokentype;
using simple::token;

int main() {
	preproc p("sample.txt");
	if (!(p.proc())) {
		system("pause");
		return 0;
	}
	lexer lex("sample.txt.pre.nodefine");
	token tok = lex.gettoken();
	while (tok.type != tokentype::END) {
		tok.print();
		tok = lex.gettoken();
	}
	system("pause");
}