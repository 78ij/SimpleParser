#include "include/parser.h"

namespace simple {
	
	tokentype parser::type() {
		tokentype ret;
		bool isunsigned = false;
		tokentype t = cur_tok.type;
		if (t == UNSIGNED) {
			isunsigned = true;
			next();
			if (t == INT) ret = UINT;
			if (t == LONG) ret = ULONG;
			if (t == SHORT) ret = USHORT;
			if (t == CHAR)ret = UCHAR;
		}
		else if (t == INT || t == CHAR || t == FLOAT || t == DOUBLE || t == SHORT || t == LONG || t == VOID) {
			ret = t;
		}
		else unexpect();
		next();
		return ret;
	}

	bool parser::istype() {
		bool ret = false;
		bool isunsigned = false;
		tokentype t = cur_tok.type;
		if (t == UNSIGNED) {
			isunsigned = true;
			next();
			if (t == INT) ret = true;
			if (t == LONG) ret = true;
			if (t == SHORT) ret = true;
			if (t == CHAR)ret = true;
		}
		else if (t == INT || t == CHAR || t == FLOAT || t == DOUBLE || t == SHORT || t == LONG || t == VOID) {
			ret = true;
		}
		return ret;
	}

	ast_node *parser::prog() {
		ast_node_prog *ret = new ast_node_prog;
		ast_node *tmp = new ast_node;
		bool back = false;
		bool isfunc = false;
		while (tmp) {
			if (cur_tok.type == END) return ret;
			if (cur_tok.type == EXTERN) {
				back = true;
				next();
			}
			type();
			accept(IDENT);
			if (cur_tok.type == LP) isfunc = true;
			unget();
			unget();
			if (back) unget();
			if (isfunc)
				tmp = func();
			else {
				tmp = vardcl();
				accept(SEMI);
			}
			ret->lst.append(tmp);
		}

		return ret;
	}

	ast_node *parser::vardcl() {
		if (cur_tok.type == END) return nullptr;
		bool ext;
		if (cur_tok.type == EXTERN) {
			ext = true;
			next();
		}
		else {
			ext = false;
		}
		tokentype t = type();
		var_dec v;
		v = var_decl();
		ast_node_vardec *ret = new ast_node_vardec;
		ret->type = t;
		ret->isext = ext;
		ret->vars.append(v);
		while (cur_tok.type == COMMA) {
			next();
			v = var_decl();
			ret->vars.append(v);
		}
		return ret;
	}
	ast_node *parser::func() {
		if (cur_tok.type == END) return nullptr;
		tokentype t = type();
		string id = accept(IDENT).val;
		accept(LP);
		list<parm_type> lp = parm_types();
		accept(RP);
		if (cur_tok.type == LC) {
			next();
			ast_node_funcdef * ret = new ast_node_funcdef;
			ast_node *tmp = new ast_node;
			ret->id = id;
			ret->ret = t;
			ret->parms = lp;
			while (tmp) {
				if (cur_tok.type == RC) break;
				if (istype()) {
					tmp = new ast_node;
					tmp = vardcl();
					ret->body.append(tmp);
					accept(SEMI);
				}
				else {
					tmp = stmt();
					ret->body.append(tmp);
				}
			}
			accept(RC);
			return ret;
		}
		else {
			ast_node_funcdec *ret = new ast_node_funcdec;
			ret->id = id;
			ret->type = t;
			ret->parms = lp;
			accept(SEMI);
			return ret;

		}
	}
	ast_node_assg *parser::assg() {
		ast_node_assg *ret = new ast_node_assg;
		ret->id = accept(IDENT).val;
		if (cur_tok.type == LB) {
			next();
			ret->isarray = true;
			ret->num = expr();
			accept(RB);
		}
		accept(ASSIGN);
		ret->right = expr();
		return ret;
	}
	ast_node *parser::stmt() {
		if (cur_tok.type == IF) {
			next();
			ast_node_if *ret = new ast_node_if;
			accept(LP);
			ast_node *cond = expr();
			ret->cond = cond;
			accept(RP);
			ast_node *body = stmt();
			ret->body = body;
			if (cur_tok.type == ELSE) {
				next();
				ast_node *el = stmt();
				ret->el = el;
			}
			return ret;
		}
		else if (cur_tok.type == WHILE) {
			next();
			ast_node_while *ret = new ast_node_while;
			accept(LP);
			ret->cond = expr();
			accept(RP);
			ret->body = stmt();
			return ret;
		}
		else if (cur_tok.type == FOR) {
			next();
			ast_node_for *ret = new ast_node_for;
			accept(LP);
			if (cur_tok.type != SEMI) {
				ret->init = assg();
			}
			accept(SEMI);
			if (cur_tok.type != SEMI) {
				ret->cond = expr();
			}
			accept(SEMI);
			if (cur_tok.type != RP) {
				ret->iter = assg();
			}
			accept(RP);
			ret->body = stmt();
			return ret;
		}
		else if (cur_tok.type == RETURN) {
			next();
			ast_node_ret *ret = new ast_node_ret;
			if (cur_tok.type != SEMI) {
				ret->stmt = expr();
			}
			accept(SEMI);
			return ret;
		}

		else if (cur_tok.type == BREAK || cur_tok.type == CONTINUE) {
			ast_node_const *ret = new ast_node_const;
			ret->tok = cur_tok;
			next();
			accept(SEMI);
			return ret;
		}
		else if (cur_tok.type == LC) {
			next();
			ast_node_bigbrac *ret = new ast_node_bigbrac;
			ast_node *tmp;
			while (cur_tok.type != RC) {
				tmp = stmt();
				ret->body.append(tmp);
			}
			accept(RC);
			return ret;
		}
		else if (cur_tok.type == IDENT) {
			next();
			if (cur_tok.type == LP) {
				unget();
				token tok = cur_tok;
				next();
				accept(LP);
				ast_node_callfunc *ret = new ast_node_callfunc;
				ret->id = tok.val;
				ret->isstmt = true;
				if (cur_tok.type != RP) {
					ast_node *tmp = expr();
					ret->params.append(tmp);
					while (cur_tok.type == COMMA) {
						next();
						tmp = expr();
						ret->params.append(tmp);
					}
				}
				accept(RP);
				accept(SEMI);
				return ret;
			}
			else {
				unget();
				ast_node_assg *ret = assg();
				accept(SEMI);
				ret->isstmt = true;
				return ret;
			}
		}
		else if (cur_tok.type == SEMI) {
			ast_node_const *ret = new ast_node_const;
			ret->tok = cur_tok;
			next();
			return ret;
		}
		else {
			unexpect();
		}
	}

	// Expression area

	int getprec(token tok) {
		if (tok.type == AND || tok.type == OR) return 1;
		if (tok.type == EQ || tok.type == NEQ) return 2;
		if (tok.type == LE || tok.type == LT || tok.type == GT || tok.type == GE) return 3;
		if (tok.type == PLUS || tok.type == MINUS) return 4;
		if (tok.type == MULTIPLY || tok.type == DIVIDE) return 5;
		else return -1;
	}

	ast_node *parser::expr() {
		//Operator precedence Climbing
		return parse_expr(expr_primary(), 0);
	}

	ast_node *parser::parse_expr(ast_node *lhs, int prec) {
		while (true) {
			int pre = getprec(cur_tok);
			token op = cur_tok;
			if (pre == -1 || pre < prec) break;
			next();
			ast_node *rhs = expr_primary();
			while (true) {
				pre = getprec(cur_tok);
				if (pre == -1 || pre <= prec) break;
				rhs = parse_expr(rhs, pre);
			}
			ast_node_bin *ret = new ast_node_bin;
			ret->type = op.type;
			ret->left = lhs;
			ret->right = rhs;
			lhs = ret;
		}
		return lhs;
	}

	ast_node *parser::expr_primary() {
		if (cur_tok.type == LP) {
			next();
			ast_node_smbrac *ret = new ast_node_smbrac;
			ret->inner = parse_expr(expr_primary(), 0);
			accept(RP);
			return ret;
		}
		if (cur_tok.type == CINT || cur_tok.type == CUINT
			|| cur_tok.type == CLONG || cur_tok.type == CULONG
			|| cur_tok.type == CCHAR || cur_tok.type == CDOUBLE
			|| cur_tok.type == CFLOAT) {
			ast_node_const *ret = new ast_node_const;
			ret->tok = cur_tok;
			next();
			return ret;
		}
		
		token tok = accept(IDENT);
		if (cur_tok.type == LP) {
			ast_node_callfunc *ret = new ast_node_callfunc;
			ret->id = tok.val;
			next();
			if (cur_tok.type != RP) {
				ast_node *tmp = expr();
				ret->params.append(tmp);
				while (cur_tok.type == COMMA) {
					next();
					tmp = expr();
					ret->params.append(tmp);
				}
			}
			accept(RP);
			return ret;
		}
		else if (cur_tok.type == LB) {
			ast_node_const *ret = new ast_node_const;
			ret->tok = tok;
			next();
			ret->isarray = true;
			ret->num = expr();
			accept(RB);
			return ret;
		}
		else {
			ast_node_const *ret = new ast_node_const;
			ret->tok = tok;
			return ret;
		}
	}

	list<parm_type> parser::parm_types() {
		list<parm_type> ret;
		tokentype t = type();
		if (t == VOID) {
			return ret;
		}
		string id = accept(IDENT).val;
		bool isarray = false;
		if (cur_tok.type == LB) {
			next();
			isarray = true;
			accept(RB);
		}
		parm_type tmp{ isarray,id,t };
		ret.append(tmp);
		while (cur_tok.type == COMMA) {
			next();
			t = type();
			id = accept(IDENT).val;
			isarray = false;
			if (cur_tok.type == LB) {
				next();
				isarray = true;
				accept(RB);
			}
			tmp.isarray = isarray;
			tmp.type = t;
			tmp.ident = id;
			ret.append(tmp);
		}
		return ret;
	}

	var_dec parser::var_decl() {
		var_dec ret;
		ret.isarray = false;
		ret.ident = accept(IDENT).val;
		if (cur_tok.type == LB) {
			next();
			ret.isarray = true;
			cur_tok.convert();
			ret.arraysize = accept(CINT).icval;
			accept(RB);
		}
		return ret;
	}

	void parser::format() {
		ofstream fout(path + ".tmp");
		root->format(0, fout);
		fout.close();
		ifstream fin(path + ".tmp");
		list<string> fmtsrc;
		string tmp;
		while (getline(fin, tmp)) {
			fmtsrc.append(tmp);
		}
		lexer lt(path + ".tmp");
		token tok;
		list<token> token_table_fmt;
		while (tok.type != tokentype::END) {
			tok = lt.gettoken();
			token_table_fmt.append(tok);
		}
		int i = token_table_comments.length - 1;
		int j = token_table_fmt.length - 1;
		list<token> com_tmp;
		for (int i = 0; i < token_table_fmt.length; i++) {
			token_table_fmt[i].print();

		}
		while (i >= 0 && j >= 0) {
			if (token_table_comments[i].type != COM) {
				i--;
				j--;
			}
			else {
				while (token_table_comments[i].type == COM) {
					com_tmp.append(token_table_comments[i--]);
				}
				token tok = token_table_fmt[j];
				int col = tok.col + tok.val.size() - 1;
				int row = tok.row;
				for (int i = 0; i < com_tmp.length; i++) {
					fmtsrc[row - 1].insert(col, " " + com_tmp[i].val);
					col += com_tmp[i].val.size();
				}
				com_tmp.clear();
			}
		}
		if (i != -1) {
			while (i >= 0) {
				fmtsrc[0].insert(0, token_table_comments[i--].val + '\n');
			}
		}
		color(2);
		cout << "Source:-------------------------\n\n";
		for (int i = 0; i < l.source.length; i++) {
			cout << l.source[i] << "\n";
		}
		color(6);
		cout << "\n";
		cout << "Formatted:----------------------\n\n";
		for (int i = 0; i < fmtsrc.length; i++) {
			cout << fmtsrc[i] << "\n";
		}
		color(263);
	}

	void parser::test() {
		
	}
}