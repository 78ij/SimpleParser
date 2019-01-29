#include "include/parser.h"

namespace simple {

	tokentype parser::type() {
		bool isunsigned = false;
		tokentype t = cur_tok.type;
		if (t == UNSIGNED) {
			isunsigned = true;
			next();
			if (t == INT) return UINT;
			if (t == LONG) return ULONG;
			if (t == SHORT) return USHORT;
			if (t == CHAR)return UCHAR;
		}
		else if (t == INT || t == CHAR || t == FLOAT || t == DOUBLE || t == SHORT || t == LONG || t == VOID) {
			return t;
		}
		else unexpect();
	}

	ast_node *parser::prog() {
		ast_node_prog *ret = new ast_node_prog;
		ast_node *tmp1;
		ast_node *tmp2;
		tmp1 = dcl();
		tmp2 = func();
		while (tmp1 || tmp2) {
			if (tmp1) {
				ret->lst.append(tmp1);
			}
			else if (tmp2) {
				ret->lst.append(tmp2);
			}
			tmp1 = dcl();
			tmp2 = func();
		}

		return ret;
	}
	ast_node *parser::dcl() {
		next();
		if (cur_tok.type == EXTERN) {
			tokentype t = type();
			next();
			if (cur_tok.type = IDENT) {
				accept(LP);
				
			}
			else {

			}
		}
		else {

		}
		return nullptr;
		
	}
	ast_node *parser::func() {
		return nullptr;
	}
	ast_node *parser::assg() {
		return nullptr;
	}
	ast_node *parser::stmt() {
		return nullptr;
	}
	ast_node *parser::expr() {
		return nullptr;
	}

	list<parser::parm_type> &parser::parm_types() {
		list<parm_type> l;
		return l;
	}

	list<parser::var_dec> &parser::var_decl() {
		list<var_dec> l;
		return l;
	}
	void parser::parse() {

	}
	
	void parser::test() {
		// int a;
		// int main(){
		// a = 1;
		// }
		ast_node_prog * _root = new ast_node_prog;
		ast_node_vardec *a = new ast_node_vardec;
		(_root->lst).append(a);
		a->id.append("a");
		a->type = INT;
		ast_node_funcdef *d = new ast_node_funcdef;
		ast_node_assg *g = new ast_node_assg;
		ast_node_const *c = new ast_node_const;
		(_root->lst).append(d);
		c->tok.type = CINT;
		c->tok.val = "1";
		g->id = "a";
		g->right = c;
		d->body = g;
		d->id = "main";
		d->ret = INT;
		root = _root;
		print();
	}
}