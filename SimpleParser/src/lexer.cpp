#include "include/lexer.h"

namespace simple {
	void token::convert() {
		switch (type) {
		case CINT:
			icval = (int)stoll(val,nullptr,0);
			break;
		case CUINT:
			iuval = (unsigned int)(stoi(val, nullptr, 0));
			break;
		case CFLOAT:
			fval = stof(val);
			break;
		case CDOUBLE:
			try {
				dval = stold(val);
			}
			catch (std::out_of_range e) {
				dval = 0;
			}
			break;
		case CCHAR:
			break;
		}
	}
	token lexer::isspsymbol(char c) {
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
		if (c == '+')
			tok.type = PLUS;
		if (c == '-')
			tok.type = MINUS;
		if (c == '*')
			tok.type = MULTIPLY;
		if (c == '/')
			tok.type = DIVIDE;
		if (c == '%')
			tok.type = MOD;
		if (c == '&') {
			char t;
			file.get(t);
			if (t == '&') {
				tok.type = AND;
				tok.val = "&&";
			}
			else {
				tok.type = ERROR;
				file.unget();
			}
		}
		if (c == '|') {
			char t;
			file.get(t);
			if (t == '|') {
				tok.type = OR;
				tok.val = "||";
			}
			else {
				tok.type = ERROR;
				file.unget();
			}
		}
		if(tok.val == "") tok.val = c;
		return tok;
	}
	token lexer::gettoken(){
		enum lexerstate {
			ERR, START, ID, DONE,INT_C,FLOAT_C, FLOAT_OR_INT, ZERO,HEX,OCTINT,E_FLOAT_C,D_FLOAT_C,
			CHAR_C,CHARERR
		};
		lexerstate state = START;
		char c;
		string tmp;
		token tok;
		while (file.get(c)) {
			if (c == '\n') {
				row++;
			}
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
				else if (isdigit(c) && c != '0') {
					state = FLOAT_OR_INT;
					tmp.push_back(c);
				}
				else if (c == '0') {
					state = ZERO;
					tmp.push_back(c);
				}
				else if (c == '\'') {
					tmp.push_back(c);
					state = CHAR_C;
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
					if (c == '\n') row--;
				}
				else {
					tmp.push_back(c);
				}
				break;
			case CHAR_C:
				char t2;
				if (c == '\'') {
					tmp.push_back(c);
					tok.type = ERROR;
					tok.val = tmp;
					state = DONE;
					if (c == '\n') row--;
				}
				else if (c == '\\') {
					tmp.push_back(c);
					file.get(t2);
					if (t2 != '\'' && !isalpha(t2) && t2 != '\\') {
						tmp.push_back(t2);
						state = CHARERR;
						break;
					}
					else {
						tmp.push_back(t2);
						file.get(t2);
						if (t2 == '\'') {
							tmp.push_back(t2);
							tok.type = CCHAR;
							tok.val = tmp;
							state = DONE;
							if (c == '\n') row--;
						}
					}
				}
				else {
					tmp.push_back(c);
					file.get(t2);
					if (t2 == '\'') {
						tmp.push_back(t2);
						tok.type = CCHAR;
						tok.val = tmp;
						state = DONE;
						if (c == '\n') row--;
					}
					else {
						tmp.push_back(t2);
						state = CHARERR;

					}
				}
				break;
			case CHARERR:
				if (c != ' ' && c != '\'') {
					tmp.push_back(c);
				}
				else {
					tmp.push_back(c);
					tok.type = ERROR;
					tok.val = tmp;
					state = DONE;
					if (c == '\n') row--;
				}
				break;
			case ZERO:
				if (c == 'x') {
					tmp.push_back(c);
					state = HEX;
				}
				else if (isdigit(c)) {
					tmp.push_back(c);
					state = OCTINT;
				}
				else if (c == '.') {
					tmp.push_back(c);
					state = FLOAT_C;
				}
				else {
					if (c == '\n') row--;
					file.unget();
					state = INT_C;
				}
				break;
			case HEX:
				if (isxdigit(c)) {
					tmp.push_back(c);
				}
				else if (c == 'E' || c == 'e') {
					tmp.push_back(c);
					state = E_FLOAT_C;
				}
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
					if (c == '\n') row--;
				}
				break;
			case OCTINT:
				if (isdigit(c) && c != '8' && c != '9') {
					tmp.push_back(c);
				}
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
					if (c == '\n') row--;
				}
				break;
			case FLOAT_OR_INT:
				if (isdigit(c))
					tmp.push_back(c);
				else if (c == 'e' || c == 'E') {
					tmp.push_back(c);
					char ftmp;
					file.get(ftmp);
					if (ftmp == '-' || ftmp == '+') {
						tmp.push_back(ftmp);
						file.get(ftmp);
						if (!isdigit(ftmp)) {
							tmp.push_back(ftmp);
							state = ERR;
							break;
						}
					}
					else if (!isdigit(ftmp)) {
						tmp.push_back(ftmp);
						state = ERR;
						break;
					}
					if (c == '\n') row--;
					file.unget();
					state = E_FLOAT_C;
				}
				else if (c == '.') {
					tmp.push_back(c);
					state = D_FLOAT_C;
				}
				else if (c == 'F' || c == 'f') {
					file.unget();
					state = FLOAT_C;
				}
				else {
					if (c == '\n') row--;
					file.unget();
					state = INT_C;
				}
				break;
			case D_FLOAT_C:
				if (isdigit(c))
					tmp.push_back(c);
				else if (c == 'e' || c == 'E') {
					tmp.push_back(c);
					char ftmp;
					file.get(ftmp);
					if (ftmp == '-' || ftmp == '+') {
						tmp.push_back(ftmp);
						file.get(ftmp);
						if (!isdigit(ftmp)) {
							tmp.push_back(ftmp);
							state = ERR;
							break;
						}
					}
					else if (!isdigit(ftmp)) {
						tmp.push_back(ftmp);
						state = ERR;
						break;
					}
					if (c == '\n') row--;
					file.unget();
					state = E_FLOAT_C;
				}
				else {
					if (c == 'f' || c == 'F') {
						tok.type = CFLOAT;
						file.get();
					}
					else tok.type = CDOUBLE;
					state = DONE;
					tok.val = tmp;
					file.unget();
					if (c == '\n') row--;
				}
				break;
			case E_FLOAT_C:
				if (isdigit(c)) tmp.push_back(c);
				else {
					if (c == 'f' || c == 'F') {
						tok.type = CFLOAT;
						file.get();
					}
					else tok.type = CDOUBLE;
					state = DONE;
					tok.val = tmp;
					file.unget();
					if (c == '\n') row--;
				}
				break;
			case FLOAT_C:
				if (isdigit(c)) tmp.push_back(c);
				else {
					if (c == 'f' || c == 'F') {
						tok.type = CFLOAT;
						file.get();
					}
					if (c == 'e' || c == 'E') {
						tmp.push_back(c);
						state = E_FLOAT_C;
						break;
					}
					else tok.type = CDOUBLE;
					state = DONE;
					tok.val = tmp;
					file.unget();
					if (c == '\n') row--;
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
					else if (c == 'l' || c == 'L') {
						tok.type = CLONG;
					}
					else tok.type = CINT;
					state = DONE;
					tok.val = tmp;
					file.unget();
					if (c == '\n') row--;
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
				if (c == '\n') row--;
				return tok;
				break;
			case ERR:
				file.unget();
				if (c == '\n') row--;
				tok.type = ERROR;
				tok.val = tmp;
				return tok;
				break;
			}
			
		}
		tok.type = END;
		return tok;
	}
}