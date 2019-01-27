#include "include/parser.h"

namespace simple {
	void parser::parse(){

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