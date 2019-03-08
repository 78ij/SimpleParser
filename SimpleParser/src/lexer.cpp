#include "include/lexer.h"

namespace simple {
	// Convert the literal values from string to value. 
	void token::convert() {
		switch (type) {
		case CLONG:
		case CINT:
			if (val[0] == '0' && val[1] != 'x') {
				if (val.find('9') != val.npos || val.find('8') != val.npos) {
					type = ERROR;
					break;
				}
			}
			icval = (int)stoll(val,nullptr,0);
			break;
		case CULONG:
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
	//Determine whether the char is a special symbol.
	token lexer::isspsymbol(char c) {
		token tok;
		tok.type = ERROR;
		tok.val = ' ';
		if (c == ',')
			tok.type = COMMA;
		else if (c == ';')
			tok.type = SEMI;
		else if (c == '<') {
			char t;
			file.get(t);
			if (t == '=') {
				tok.type = LE;
				tok.val = "!=";
			}
			else {
				tok.type = LT;
				file.unget();
			}
		}
		else if (c == '>') {
			char t;
			file.get(t);
			if (t == '=') {
				tok.type = GE;
				tok.val = "!=";
			}
			else {
				tok.type = GT;
				file.unget();
			}
		}
		else if (c == '!') {
			char t;
			file.get(t);
			if (t == '=') {
				tok.type = NEQ;
				tok.val = "!=";
			}
			else {
				tok.type = NOT;
				file.unget();
			}
		}
		else if (c == '(')
			tok.type = LP;
		else if (c == ')')
			tok.type = RP;
		else if (c == '[')
			tok.type = LB;
		else if (c == ']')
			tok.type = RB;
		else if (c == '{')
			tok.type = LC;
		else if (c == '}')
			tok.type = RC;
		else if (c == '+')
			tok.type = PLUS;
		else if (c == '-')
			tok.type = MINUS;
		else if (c == '*')
			tok.type = MULTIPLY;
		else if (c == '/')
			tok.type = DIVIDE;
		else if (c == '%')
			tok.type = MOD;
		else if (c == '&') {
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
		else if (c == '|') {
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
		else if (c == '!') {
			char t;
			file.get(t);
			if (t == '=') {
				tok.type = NEQ;
				tok.val = "!=";
			}
			else {
				tok.type = ERROR;
				file.unget();
			}
		}
		else if (c == '=') {
			char t;
			file.get(t);
			if (t == '=') {
				tok.type = EQ;
				tok.val = "==";
			}
			else {
				tok.type = ASSIGN;
				file.unget();
			}
		}
		else if (tok.val == "") tok.val = c;
		return tok;
	}
	// MASSIVE STATE MACHINE!!!!!!
	token lexer::gettoken(){
		enum lexerstate {
			ERR, START, ID, DONE,INT_C,FLOAT_C, FLOAT_OR_INT, ZERO,HEX,OCTINT,E_FLOAT_C,D_FLOAT_C,
			CHAR_C,CHARERR,STRING,STRINGERR, /*Man,i really dont want to add this*/ SLASHCOMMENT,ASTERISKCOMMENT,PREPROC
		};
		lexerstate state = START;
		char c;
		string tmp;
		token tok;
		while (file.get(c)) {
			col++;
			if (c == '\n') {
				row++;
				col = 0;
			}
			switch (state) {
			case START:
				if (c == '/') {
					tmp.push_back(c);
					file.get(c);
					if (c == '/') {
						tmp.push_back(c);
						state = SLASHCOMMENT;
						break;
					}
					else if (c == '*') {
						tmp.push_back(c);
						state = ASTERISKCOMMENT;
						break;
					}
					else {
						file.unget();
						c = '/';
					}
				}
				tok = isspsymbol(c);
				tok.row = row;
				tok.col = col;
				if (tok.type != ERROR) return tok;
				if (isspace(c)) continue;
				else if (isalpha(c) || c == '_') {
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
				else if (c == '#') {
					if (col == 1) {
						state = PREPROC;
						tmp.push_back(c);
					}
					else {
						state = ERR;
						tmp.push_back(c);
					}
				}
				else if (c == '\"') {
					state = STRING;
				}
				else {
					state = ERR;
					tmp.push_back(c);
				}
				break;
			
			case PREPROC:
				if (c == '\n') {
					state = DONE;
					tok.type = COM; // Handle PreProc as comments(temp)
					tok.val = tmp;
					file.unget();
					row--;
					col = source[row - 1].size();
				}
				else
					tmp.push_back(c);
				break;
			case SLASHCOMMENT:
				if (c == '\n') {
					state = DONE;
					tok.type = COM;
					tok.val = tmp;
					file.unget();
					row--;
					col = source[row - 1].size();
				}
				else
					tmp.push_back(c);
				break;
			case ASTERISKCOMMENT:
				if (c == '*') {
					file.get(c);
					if (c == '/') {
						tmp += "*/ ";

						state = DONE;
						tok.type = COM;
						tok.val = tmp;
						break;
					}
					file.unget();
					tmp.push_back('*');
				}
				else
					tmp.push_back(c);
				break;
			case ID:
				if (!(isalnum(c)) || c == '_') {
					state = DONE;
					col--;
					tok.type = IDENT;
					tok.val = tmp;
					file.unget();
					if (c == '\n') {
						row--;
						col = source[row - 1].size();
					}
				}
				else {
					tmp.push_back(c);
				}
				break;
			case STRING:
				if (c != '\\' && c != '\"' && c != '\n') tmp.push_back(c);
				else if (c == '\\') {
					tmp.push_back(c);
					file.get(c);
					if (c != '\'' && !isalpha(c) && c != '\\') {
						tmp.push_back(c);
						state = STRINGERR;
						break;
					}
					else tmp.push_back(c);
				}
				else if (c == '\"') {
					tok.type = CSTRING;
					tok.val = tmp;
					state = DONE;
					col--;
					if (c == '\n') {
						file.unget();
						row--;
						col = source[row - 1].size();
					}
				}
				else if (c == ' ' || c == '\n') {
					tok.type = ERROR;
					tok.val = tmp;
					state = DONE;
					col--;
					if (c == '\n') {
						file.unget();
						row--;
						col = source[row - 1].size();
					}
				}
				break;
			case STRINGERR:
				if (c != ' ' && c != '\n') {
					tmp.push_back(c);
				}
				else {
					tmp.push_back(c);
					tok.type = ERROR;
					tok.val = tmp;
					state = DONE;
					col--;
					if (c == '\n') {
						file.unget();
						row--;
						col = source[row - 1].size();
					}
				}
				break;
			case CHAR_C:
				char t2;
				if (c == '\'') {
					tmp.push_back(c);
					tok.type = ERROR;
					tok.val = tmp;
					state = DONE;

				}
				else if (c == '\\') {
					tmp.push_back(c);
					file.get(t2);
					col++;
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
						}
					}
				}
				else {
					tmp.push_back(c);
					col++;
					file.get(t2);
					if (t2 == '\'') {
						tmp.push_back(t2);
						tok.type = CCHAR;
						tok.val = tmp;
						state = DONE;
						col++;
					}
					else {
						tmp.push_back(t2);
						state = CHARERR;

					}
				}
				break;
			case CHARERR:
				if (c != ' ' && c != '\'' && c != '\n') {
					tmp.push_back(c);
				}
				else {
					tok.type = ERROR;
					tok.val = tmp;
					state = DONE;
					col--;
					if (c == '\n') {
						file.unget();
						row--;
						col = source[row - 1].size();
					}
				}
				break;
			case ZERO:
				if (c == 'x') {
					tmp.push_back(c);
					state = HEX;
				}
				else if (isdigit(c)) {
					tmp.push_back(c);
					state = FLOAT_OR_INT;
				}
				else if (c == '.') {
					tmp.push_back(c);
					state = FLOAT_C;
				}
				else {
					if (c == '\n') {
						file.unget();
						row--;
						col = source[row - 1].size();
					}
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
				else if (c == 'l' || c == 'L') {
					tok.type = CLONG;
					state = DONE;
					tok.val = tmp;

				}
				else {
					if (c == 'u' || c == 'U') {
						char t;
						file.get(t);
						if (t == 'l' || t == 'L') {
							col += 1;
							tok.type = CULONG;
						}
						else {
							file.unget();
							tok.type = CUINT;
						}
						file.get();
					}
					else {
						tok.type = CINT;
						col--;
					}
					state = DONE;
					tok.val = tmp;
					file.unget();
					if (c == '\n') {
						row--;
						col = source[row - 1].size();
					}
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
							col += 1;
						}
						else {
							file.unget();
							tok.type = CUINT;
						}
						file.get();
					}
					else {
						tok.type = CINT;
						col--;
					}
					state = DONE;
					tok.val = tmp;
					file.unget();
					if (c == '\n') {
						row--;
						col = source[row - 1].size();
					}
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
					if (c == '\n') {
						row--;
						col = source[row - 1].size();
					}
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
					if (c == '\n') {
						row--;
						col = source[row - 1].size();
					}
					col--;
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
					if (c == '\n') {
						file.unget();
						row--;
						col = source[row - 1].size();
					}
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
					col--;
					tok.val = tmp;
					file.unget();
					if (c == '\n') {
						row--;
						col = source[row - 1].size();
					}
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
					col--;
					tok.val = tmp;
					file.unget();
					if (c == '\n') {
						row--;
						col = source[row - 1].size();
					}
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
					col--;
					tok.val = tmp;
					file.unget();
					if (c == '\n') {
						row--;
						col = source[row - 1].size();
					}
				}
				break;
			case INT_C:
				if (isdigit(c))
					tmp.push_back(c);
				else {
					if (c == 'u' || c == 'U') {
						char t;
						file.get(t);
						if (t == 'l' || t == 'L') {
							tok.type = CULONG;
							col += 1;
						}
						else {
							file.unget();
							tok.type = CUINT;

						}
						file.get();
						state = DONE;
						tok.val = tmp;
						file.unget();
						if (c == '\n') {

							row--;
							col = source[row - 1].size();
						}
					}
					else if (c == 'l' || c == 'L') {
						file.get();
						tok.type = CLONG;
						state = DONE;
						col--;
						tok.val = tmp;
						file.unget();
						if (c == '\n') {
							row--;
							col = source[row - 1].size();
						}
					}
					else {
						tok.type = CINT;
						state = DONE;
						col--;
						tok.val = tmp;
						file.unget();
						if (c == '\n') {
							row--;
							col = source[row - 1].size();
						}
					}
				}
				break;
			case DONE:
				if (tok.type == IDENT) {
					if (tok.val == "if") tok.type = IF;
					if (tok.val == "int") tok.type = INT;
					if (tok.val == "float") tok.type = FLOAT;
					if (tok.val == "double") tok.type = DOUBLE;
					if (tok.val == "char") tok.type = CHAR;
					if (tok.val == "short") tok.type = SHORT;
					if (tok.val == "long") tok.type = LONG;
					if (tok.val == "else") tok.type = ELSE;
					if (tok.val == "while") tok.type = WHILE;
					if (tok.val == "for") tok.type = FOR;
					if (tok.val == "return") tok.type = RETURN;
					if (tok.val == "break") tok.type = BREAK;
					if (tok.val == "extern") tok.type = EXTERN;
					if (tok.val == "continue") tok.type = CONTINUE;
					if (tok.val == "unsigned") tok.type = UNSIGNED;
					if (tok.val == "void") tok.type = VOID;
				}


				if (c == '\n') {
					row--;
					col = source[row - 1].size();
					tok.col = col - tok.val.size() + 1;
				}
				else {
					col--;
					tok.col = col - tok.val.size() + 1;
				}
				tok.row = row;
				if (tok.type == COM) {
					int c = 0;
					for (int i = 0; i < tmp.length(); i++) {
						if (tmp[i] == '\n') c++;
					}
					tok.row -= c;
				}
				file.unget();
				
				return tok;
				break;
			case ERR:
				if (c != ' ' && c != '\n') {
					tok.type = ERROR;
					tmp.push_back(c);
				}
				else {
					file.unget();
					if (c == '\n') {
						row--;
						col = source[row - 1].size();
					}
					tok.type = ERROR;
					tok.row = row;
					tok.col = col;
					tok.val = tmp;
					return tok;
				}
				
				break;
			}
			
		}
		tok.type = END;
		tok.row = row;
		tok.col = col;
		return tok;
	}
}