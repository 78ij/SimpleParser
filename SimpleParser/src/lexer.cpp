#include "include/lexer.h"

namespace simple {
	token isspsymbol(char c) {
		token tok;
		tok.type = ERROR;
		if (c == ';')
			tok.type = SEMI;
		if (c == '<') 
			tok.type = LT;
		if (c == '>') 
			tok.type = GT;
		if (c == '=')
			tok.type = ASSIGN;
		if (c == '(')
			tok.type = LP;
		if (c == ')')
			tok.type = RP;
		if (c == '[')
			tok.type = LB;
		if (c == ']')
			tok.type = RB;
		if (c == '{')
			tok.type = LC;
		if (c == '}')
			tok.type = RC;
		tok.val = c;
		return tok;
	}
	token lexer::gettoken(){
		enum lexerstate {
			ERR, START, ID, DONE,INT_C,FLOAT_C, FLOAT_OR_INT
		};
		lexerstate state = START;
		char c;
		string tmp;
		token tok;
		while (file.get(c)) {
			switch (state) {
			case START:
				tok = isspsymbol(c);
				if (tok.type != ERROR) return tok;
				if (isspace(c)) continue;
				else if (isalpha(c)) {
					state = ID;
					tmp.push_back(c);
				}
				else if (c == '.') {
					state = FLOAT_C;
					tmp.push_back(c);
				}
				else if (isdigit(c)) {
					state = FLOAT_OR_INT;
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
			case FLOAT_OR_INT:
				if (isdigit(c))
					tmp.push_back(c);
				else if (c == 'e') {
					tmp.push_back(c);
					state = FLOAT_C;
				}
				else if (c == '.') {
					tmp.push_back(c);
					state = FLOAT_C;
				}
				else {
					file.unget();
					state = INT_C;
				}
				break;
			case INT_C:
				if (isdigit(c))
					tmp.push_back(c);
				else {
					if (c == 'u' || c == 'U') {
						char tmp;
						file.get(tmp);
						if (tmp == 'l' || tmp == 'L') {
							tok.type = CULONG;
						}
						else {
							file.unget();
							tok.type = CUINT;
						}
						file.get();
					}
					else tok.type = CINT;
					state = DONE;
					tok.val = tmp;
					file.unget();
				}
				break;
			case DONE:
				if (tok.type == IDENT) {
					if (tok.val == "if") tok.type = IF;
					if (tok.val == "else") tok.type = ELSE;
					if (tok.val == "while") tok.type = WHILE;
					if (tok.val == "for") tok.type = FOR;
					if (tok.val == "return") tok.type = RETURN;
					if (tok.val == "break") tok.type = BREAK;
					if (tok.val == "extern") tok.type = EXTERN;
					if (tok.val == "continue") tok.type = CONTINUE;
					if (tok.val == "unsigned") tok.type = UNSIGNED;
				}
				file.unget();
				return tok;
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