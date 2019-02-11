#ifndef SIMP_PAR_H
#define SIMP_PAR_H

#include "include/lexer.h"


namespace simple {
	
	inline void pl(int l) {
		for (int i = 0; i < l; i++)
			cout << '\t';
	}

	struct parm_type {
		bool isarray;
		string ident;
		tokentype type;
	};

	struct var_dec {
		bool isarray;
		int arraysize;
		string ident;
	};

	class ast_node {
	public:
		virtual void print(int l) {}
	};

	class ast_node_prog :public ast_node{
	public:
		list<ast_node*> lst;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Prog  Childs:\n";
			for (int i = 0; i < lst.length; i++) {
				lst[i]->print(l + 1);
			}
			cout << "\n";
		}
	};

	//id, const,  break, continue
	class ast_node_const : public ast_node {
	public:
		token tok;
		bool isarray = false;
		ast_node *num;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Const  " << "Token Information: ";
			tok.print();
		}
	};

	class ast_node_bigbrac : public ast_node {
	public:
		list<ast_node *> body;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Big Bracket Childs:\n";
			for (int i = 0; i < body.length; i++) {
				body[i]->print(l + 1);
			}
		}
	};

	class ast_node_smbrac : public ast_node {
	public:
		ast_node *inner;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Small Bracket Child:\n";
			inner->print(l + 1);
		}
	};
	class ast_node_if : public ast_node {
	public:
		ast_node *cond;
		ast_node *body;
		ast_node *el;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: If\n";
			cout << "Condition:\n";
			cond->print(l + 1);
			cout << "Body:\n";
			body->print(l + 1);
			if (el != nullptr) {
				cout << "Else:\n";
				el->print(l + 1);
			}
			cout << "\n";
		}
	};

	class ast_node_while : public ast_node{
	public:
		ast_node *cond;
		ast_node *body;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: While\n";
			cout << "Condition:\n";
			cond->print(l  + 1);
			cout << "body:\n";
			body->print(l + 1);
			cout << "\n";

		}
	};

	class ast_node_for : public ast_node {
	public:
		ast_node *init;
		ast_node *cond;
		ast_node *iter;
		ast_node *body;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: For\n";
			if (init != nullptr) {
				cout << "Init:\n";
				init->print(l + 1);
			}
			if (cond != nullptr) {
				cout << "Cond:\n";
				cond->print(l + 1);
			}
			if (iter != nullptr) {
				cout << "Iter\n";
				iter->print(l + 1);
			}
			cout << "Body:\n";
			body->print(l + 1);
			cout << "\n";

		}
	};

	class ast_node_ret : public ast_node {
	public:
		ast_node *stmt;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Return\n";
			cout << "Statement:\n";
			stmt->print(l + 1);
		}
	};

	class ast_node_parstmt : public ast_node {
		ast_node *stmt;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Parentheses Statement Statement:\n";
			stmt->print(l + 1);
		}
    };

	class ast_node_callfunc : public ast_node {
	public:
		string id;
		list<ast_node *> params;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Function Call\n";
			for (int i = 1; i <= params.length; i++) {
				cout << "Parameter " << i << " :\n";
				params[i]->print(l + 1);
			}
			cout << "\n";
		}
	};

	class ast_node_funcdec : public ast_node {
	public:
		tokentype type;
		string id;
		list<parm_type> parms;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Function Declare, id: " << id << "Returns: " << tokens[type] << "Types: ";
			for (int i = 0; i < parms.length - 1; i++)
				cout << tokens[parms[i].type] << ", ";
			cout << tokens[parms[parms.length - 1].type] << "\n";
		}
	};

	class ast_node_funcdef :public ast_node {
	public:
		tokentype ret;
		string id;
		list<parm_type> parms;
		list<ast_node*> body;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Function Define, id: " << id << "Returns: " << tokens[ret] << " Types: ";
			if (parms.length > 0) {
				for (int i = 0; i < (parms.length - 1); i++)
					cout << tokens[parms[i].type] << ", ";
				cout << tokens[parms[parms.length - 1].type] << "  ";
			}
			else cout << "None";
			cout << "body:\n";
			if (body.length > 0) {
				for (int i = 0; i < body.length; i++)
					body[i]->print(l + 1);
			}
		}
	};

	class ast_node_vardec : public ast_node {
	public:
		bool isext;
		tokentype type;
		list<var_dec> vars;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Variable Declare, type: " << tokens[type]  << "id: ";
				for (int i = 0; i < vars.length - 1; i++)
					cout << vars[i].ident << ", ";
			cout << vars[vars.length - 1].ident << "\n";
		}
	};

	class ast_node_bin : public ast_node {
	public:
		tokentype type;
		ast_node *left;
		ast_node *right;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Binary Operation, type:" << tokens[type] << "left:\n";
			left->print(l + 1);
			cout << "right:\n";
			right->print(l + 1);
		}
	};

	class ast_node_assg : public ast_node {
	public:
		string id;
		bool isarray = false;
		ast_node *num;
		ast_node *right;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Assignment, id: " << id << "right:\n";
			right->print(l + 1);
		}
	};

	class parser {
	public:
		void parse();
		void print() {
			root->print(0);
		}
		void test();
		string path;
		list<token> token_table;
		parser(string _path) : path(_path), l(_path){
			token tok = l.gettoken();
			token_table.append(tok);
			while (tok.type != tokentype::END) {
				tok = l.gettoken();
				token_table.append(tok);
			}

			token_table.append(tok); // Make sure EOF is properly recognized
			cur_tok = token_table[0];
		}
	private:

		tokentype type();
		ast_node *prog();
		ast_node *func();
		ast_node *vardcl();
		ast_node *stmt();
		ast_node *assg();

		ast_node *expr();
		//Aux function of expression parsing
		ast_node *expr_primary();
		ast_node *parse_expr(ast_node *lhs,int prec);

		list<parm_type> parm_types();
		var_dec var_decl();
		
		lexer l;
		ast_node *root;
		int cur_tokpos = 0;
		token cur_tok;
		bool istype();
		token accept(tokentype tok) {
			if (cur_tok.type == tok) {
				token tmp = cur_tok;
				next();
				return tmp;
			}
			else {
				unexpect();
			}
		}
		void unexpect() {
			error e(cur_tok.row, cur_tok.col, "Unexpected token", l.source[cur_tok.row - 1]);
			e.print();
			system("pause");
			exit(-1);
		}
		void next() {
			cur_tok = token_table[++cur_tokpos];
		}
		void unget() {
			token_table[cur_tokpos--] = cur_tok;
			cur_tok = token_table[cur_tokpos];
		}
	};

}


#endif // SIMP_PAR_H