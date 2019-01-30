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
		if (cur_tok.type == RP) {
			next();
			ast_node_funcdec *ret = new ast_node_funcdec;
			ret->id = id;
			ret->type = t;
			return ret;
		}
		else {
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
	}
	ast_node *parser::assg() {
		string id = accept(IDENT).val;
		if (cur_tok.type = LB) {

		}
		accept(ASSIGN);
	}
	ast_node *parser::stmt() {
		if (cur_tok.type == IF) {
			next();
			accept(LP);
		}
		else if (cur_tok.type == WHILE) {
			next();
			accept(LP);
			accept(RP);
		}
		else if (cur_tok.type == FOR) {
			next();
			accept(LP);
			accept(SEMI);
			accept(SEMI);
			accept(RP);
		}
		else if (cur_tok.type == RETURN) {
			accept(SEMI);
		}

		else if (cur_tok.type == CONTINUE || cur_tok.type == CONTINUE) {
			next();
			ast_node_const *ret = new ast_node_const;
			ret->tok = cur_tok;
			accept(SEMI);
			return ret;
		}
		else if (cur_tok.type == SEMI) {

		}
	}
	ast_node *parser::expr() {
		return nullptr;
	}

	list<parm_type> parser::parm_types() {
		list<parm_type> ret;
		tokentype t = type();
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
			ret.arraysize = accept(CINT).icval;
			accept(RB);
		}
		return ret;
	}
	void parser::parse() {
		root = prog();
	}
	
	void parser::test() {
		
	}
}