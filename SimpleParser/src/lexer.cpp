#include "include/lexer.h"

namespace simple {
	token lexer::gettoken(){
		enum lexerstate {
			ERR, START, ID, DONE
		};
		lexerstate state = START;
		char c;
		string tmp;
		token tok;
		while (file.get(c)) {
			switch (state) {
			case START:
				if (isspace(c)) continue;
				else if (isalpha(c)) {
					state = ID;
					tmp.push_back(c);
				}
				else {
					state = ERR;
					tmp.push_back(c);
				}
				break;
			case ID:
				if (!(isalnum(c))) {
					state = DONE;
					tok.type = IDENT;
					tok.val = tmp;
					file.unget();
				}
				else {
					tmp.push_back(c);
				}
				break;
			case DONE:
				return tok;
				file.unget();
			case ERR:
				file.unget();
				tok.type = ERROR;
				tok.val = tmp;
				return tok;
			}
			
		}
		tok.type = END;
		return tok;
	}
}