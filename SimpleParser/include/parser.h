#ifndef SIMP_PAR_H
#define SIMP_PAR_H

#include "include/lexer.h"

namespace simple {
	
	inline void pl(int l) {
		for (int i = 0; i < l; i++)
			cout << '\t';
	}

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
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Const  " << "Token Information: ";
			tok.print();
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
		bool isextern;
		tokentype ret;
		string id;
		list<tokentype> types;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Function Declare, id: " << id << "Returns: " << tokens[ret] << "Types: ";
			for (int i = 0; i < types.length - 1; i++)
				cout << tokens[types[i]] << ", ";
			cout << tokens[types[types.length - 1]] << "\n";
		}
	};

	class ast_node_funcdef :public ast_node {
	public:
		bool isext;
		tokentype ret;
		string id;
		list<tokentype> types;
		ast_node *body;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Function Define, id: " << id << "Returns: " << tokens[ret] << " Types: ";
			if (types.length > 0) {
				for (int i = 0; i < (types.length - 1); i++)
					cout << tokens[types[i]] << ", ";
				cout << tokens[types[types.length - 1]] << "  ";
			}
			else cout << "None";
			cout << "body:\n";
			body->print(l + 1);
		}
	};

	class ast_node_vardec : public ast_node {
	public:
		bool isext;
		tokentype type;
		list<string> id;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Variable Declare, type: " << tokens[type]  << "id: ";
				for (int i = 0; i < id.length - 1; i++)
					cout << id[i] << ", ";
			cout << id[id.length - 1] << "\n";
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
			token eof;
			eof.type = END;
			token_table.append(eof); // Make sure EOF is properly recognized
		}
	private:
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
		ast_node *prog();
		ast_node *dcl();
		ast_node *stmt();
		ast_node *func();
		ast_node *assg();
		ast_node *expr();
		list<parm_type> &parm_types();
		list<var_dec> &var_decl();
		
		lexer l;
		ast_node *root;
		int cur_tokpos = 0;
		token cur_tok;
		int accept(token tok) {
			if (tok.type == cur_tok.type) {
				next();
			}
		}
		void expect(tokentype type) {
			next();
			if (cur_tok.type != type) {
				error e();
			}
		}
		void next() {
			cur_tok = token_table[cur_tokpos++];
		}
		void unget(token tok) {
			token_table[cur_tokpos--] = tok;
		}
	};

}


#endif // SIMP_PAR_H