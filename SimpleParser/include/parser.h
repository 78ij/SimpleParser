#ifndef SIMP_PAR_H
#define SIMP_PAR_H

#include "include/lexer.h"


namespace simple {
	
	inline string mytolower(string s) {
		string ret;
		for (auto c : s)
			ret.push_back(tolower(c));
		return ret;
	}

	inline void pl(int l) {
		for (int i = 0; i < l * 2; i++)
			cout << ' ';
	}

	inline void pl(int l,ostream &out) {
		for (int i = 0; i < l * 4; i++)
			out << ' ';
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
		bool isinfunc = false; // Compound Statement
		virtual void print(int l) {}
		virtual void format(int l,ostream &out) {}
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
		void format(int l,ostream &out) override {
			pl(l,out);
			for (int i = 0; i < lst.length; i++) {
				lst[i]->format(l,out);
			}
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
		void format(int l, ostream &out) override {
			if (tok.type == BREAK) {
				pl(l, out);
				out << "break;\n";
			}
			else if (tok.type == CONTINUE) {
				pl(l, out);
				out << "continue;\n";
			}
			else if (tok.type == SEMI) {
				pl(l, out);
				out << ";\n";
			}
			else if (tok.type == CCHAR) {
				out << tok.val;
			}
			else if (tok.type == CSTRING) {
				out << "\"" << tok.val << "\"";
			}
			else if (tok.type == CUINT)
				out << tok.val << "u";
			else if (tok.type == CLONG)
				out << tok.val << "l";
			else if (tok.type == CULONG)
				out << tok.val << "ul";
			else
				out << tok.val << "";
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
		void format(int l,ostream &out) override {
			if (isinfunc)
				pl(l - 1, out);
			else
				pl(l, out);
			out << "{\n";
			for (int i = 0; i < body.length; i++) {
				if (isinfunc)
					body[i]->format(l, out);
				else
					body[i]->format(l + 1, out);
			}
			if (isinfunc)
				pl(l - 1, out);
			else 
				pl(l, out);
			out << "}\n";
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
		void format(int l,ostream &out) override {
			out << "(";
			inner->format(l + 1,out);
			out << ")";
		}
	};
	class ast_node_if : public ast_node {
	public:
		ast_node *cond;
		ast_node *body;
		list<ast_node *>elif;
		list<ast_node *>elifcond;
		ast_node *el;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: If\n";
			pl(l);
			cout << "Condition:\n";
			cond->print(l + 1);
			pl(l);
			cout << "Body:\n";
			body->print(l + 1);
			for (int i = 0; i < elif.length; i++) {
				pl(l);
				cout << "Else IF, Condition:\n";
				elifcond[i]->print(l + 1);
				pl(l);
				cout << " Body:\n";
				
				elif[i]->print(l + 1);
			}
			if (el != nullptr) {
				pl(l);
				cout << "Else:\n";
				el->print(l + 1);
			}
			
		}
		void format(int l, ostream &out) override {
			pl(l, out);
			out << "if(";
			cond->format(l + 1,out);
			out << ") \n";
			body->isinfunc = true;
			body->format(l + 1,out);
			for (int i = 0; i < elif.length; i++) {
				pl(l, out);
				out << "else if(";
				elifcond[i]->format( l + 1,out);
				out << ")\n";
				elif[i]->isinfunc = true;
				elif[i]->format(l + 1, out);
			}
			if (el != nullptr) {
				pl(l, out);
				el->isinfunc = true;
				out << "else\n";
				el->format(l + 1,out);
			}
			
		}
	};

	class ast_node_while : public ast_node{
	public:
		ast_node *cond;
		ast_node *body;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: While\n";
			pl(l);
			cout << "Condition:\n";
			cond->print(l  + 1);
			pl(l);
			cout << "body:\n";
			body->print(l + 1);
		}
		void format(int l, ostream &out) override {
			pl(l, out);
			out << "while(";
			cond->format(l + 1,out);
			out << ") \n";
			body->isinfunc = true;
			body->format(l + 1,out);
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
				pl(l);
				cout << "Init:\n";
				init->print(l + 1);
			}
			if (cond != nullptr) {
				pl(l);
				cout << "Cond:\n";
				cond->print(l + 1);
			}
			if (iter != nullptr) {
				pl(l);
				cout << "Iter\n";
				iter->print(l + 1);
			}
			pl(l);
			cout << "Body:\n";

			body->print(l + 1);
		}
		void format(int l, ostream &out) override {
			pl(l, out);
			out << "for(";
			if (init != nullptr) {
				init->format(l + 1,out);
			}
			out << "; ";
			if (cond != nullptr) {
				cond->format(l + 1,out);
			}
			out << "; ";
			if (iter != nullptr) {
				iter->format(l + 1,out);
			}
			out << ")\n";
			body->isinfunc = true;
			body->format(l + 1,out);
		}
	};

	class ast_node_ret : public ast_node {
	public:
		ast_node *stmt;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Return\n";
			pl(l);
			cout << "Statement:\n";
			if(stmt != nullptr)
				stmt->print(l + 1);
		}
		void format(int l, ostream &out) {
			pl(l, out);
			out << "return ";
			if (stmt != nullptr)
				stmt->format(l + 1,out);
			out << "; \n";
		}
	};

	//class ast_node_parstmt : public ast_node {
	//	ast_node *stmt;
	//	void print(int l) override {
	//		pl(l);
	//		cout << "ASTNode Type: Parentheses Statement :\n";
	//		stmt->print(l + 1);
	//	}
 //   };

	class ast_node_callfunc : public ast_node {
	public:
		string id;
		bool isstmt = false;
		list<ast_node *> params;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Function Call\n";
			for (int i = 0; i < params.length; i++) {
				pl(l + 1);
				cout << "Parameter " << i << " :\n";
				params[i]->print(l + 1);
			}
			cout << "\n";
		}
		void format(int l, ostream &out) {
			if (isstmt) {
				pl(l, out);
				out << id << "(";
				for (int i = 0; i < params.length - 1; i++) {
					params[i]->format(l + 1,out);
					out << ",";
				}
				params[params.length - 1]->format(l + 1,out);
				out << ");\n";
			}
			else {
				out << id << "(";
				for (int i = 0; i < params.length - 1; i++) {
					params[i]->format(l + 1,out);
					out << ",";
				}
				params[params.length - 1]->format(l + 1,out);
				out << ")";
			}
		}
	};

	class ast_node_funcdec : public ast_node {
	public:
		tokentype type;
		string id;
		list<parm_type> parms;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Function Declare, id: " << id << "Returns: " << tokens[type] << " Types: ";
			for (int i = 0; i < parms.length - 1; i++)
				cout << tokens[parms[i].type] << ", ";
			cout << tokens[parms[parms.length - 1].type] << "\n";
		}
		void format(int l, ostream &out) override {
			pl(l, out);
			out << mytolower(tokens[type]) << " ";
			out << id << "(";
			if (parms.length == 0) out << "void";
			if (parms.length > 0) {
				for (int i = 0; i < parms.length - 1; i++) {
					out << mytolower(tokens[parms[i].type]) << " ";
					out << parms[i].ident;
					if (parms[i].isarray) {
						cout << "[] ";
					}
					out << ",";
				}
				out << mytolower(tokens[parms[parms.length - 1].type]) << " ";
				out << parms[parms.length - 1].ident;
				if (parms[parms.length - 1].isarray) {
					out << "[] ";
				}
			}

			out << ");\n";
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
		void format(int l, ostream &out) override {
			pl(l, out);
			out << mytolower(tokens[ret]) << " ";
			out << id << "(";
			if (parms.length == 0) out << "void";
			if (parms.length > 0) {
				for (int i = 0; i < parms.length - 1; i++) {
					out << mytolower(tokens[parms[i].type]) << " ";
					out << parms[i].ident;
					if (parms[i].isarray) {
						out << "[] ";
					}
					out << ",";
				}
				out << mytolower(tokens[parms[parms.length - 1].type]) << " ";
				out << parms[parms.length - 1].ident;
				if (parms[parms.length - 1].isarray) {
					out << "[] ";
				}
			}
			out << ")";
			out << "\n{\n";
			for (int i = 0; i < body.length; i++) {
				body[i]->format(l + 1,out);
			}
			out << "}\n";
		}
	};

	class ast_node_vardec : public ast_node {
	public:
		bool isext;
		tokentype type;
		list<var_dec> vars;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Variable Declare, type: " << tokens[type]  << " id: ";
				for (int i = 0; i < vars.length - 1; i++)
					cout << vars[i].ident << ", ";
			cout << vars[vars.length - 1].ident << "\n";
		}
		void format(int l, ostream &out) override {
			pl(l, out);
			if (isext) out << "extern ";
			out << mytolower(tokens[type]) << " ";
			for (int i = 0; i < vars.length - 1; i++) {
				out << vars[i].ident;
				if (vars[i].isarray) {
					out << "[";
					out << vars[i].arraysize;
					out << "]";
				}
				out << ", ";
			}
			out << vars[vars.length - 1].ident;
			if (vars[vars.length - 1].isarray) {
				out << "[";
				out << vars[vars.length - 1].arraysize;
				out << "]";
			}
			out << ";\n";
		}
	};

	class ast_node_bin : public ast_node {
	public:
		tokentype type;
		ast_node *left;
		ast_node *right;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Binary Operation, type:" << tokens[type] << "\n";
			pl(l);
			cout <<	"left:\n";
			left->print(l + 1);
			pl(l);
			cout << "right:\n";
			right->print(l + 1);
		}
		void format(int l, ostream &out) override {
			left->format(l + 1,out);
			out << " " << tokens[type + 27] << " ";
			right->format(l + 1,out);
		}
	};
	class ast_node_exprstmt : public ast_node{
	public:
		ast_node *exp;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Expression Statement, expression:\n";
			exp->print(l + 1);
		}
		void format(int l, ostream &out )override {
			pl(l,out);
			exp->format(l,out);
			out << ";\n";
		}
	};

	class ast_node_assg : public ast_node {
	public:
		string id;
		bool isstmt = false;
		bool isarray = false;
		ast_node *num;
		ast_node *right;
		void print(int l) override {
			pl(l);
			cout << "ASTNode Type: Assignment, id: " << id << " right:\n";
			right->print(l + 1);
		}
		void format(int l, ostream &out) override {
			if(isstmt)
				pl(l, out);
			out << id;
			if (isarray) {
				out << "[";
				num->format(l + 1,out);
				out << "]";
			}
			out << " = ";
			right->format(l + 1,out);
			if (isstmt) {
				out << ";\n";
			}
		}
	};

	class parser {
	public:
		void parse() {
			root = prog();
		}
		void format(string filename);
		void print() {
			root->print(0);
		}
		void test();
		string path;
		list<token> token_table;
		list<token> token_table_comments;
		parser(string _path) : path(_path), l(_path){
			token tok = l.gettoken();
			token_table_comments.append(tok);
			while (tok.type != tokentype::END) {
				tok = l.gettoken();
				token_table_comments.append(tok);
			}

			//Comments are taken out
			token tmp;
			for (int i = 0; i < token_table_comments.length;i++) {
				tmp = token_table_comments[i];
				if (tmp.type != COM) token_table.append(tmp);
			}
			cur_tok = token_table[0];

		}
	private:

		tokentype type();
		ast_node *prog();
		ast_node *func();
		ast_node *vardcl();
		ast_node *stmt();
		ast_node_assg *assg();

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