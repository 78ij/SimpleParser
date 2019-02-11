##  The Syntax of the C-like language : EBNF



prog		 := {vardcl SEMI | funcdecl SEMI | func}

vardcl	         := [EXTERN] type var_decl { COMMA var_decl}

funcdecl          := type IDENT LP [parm_types] RP 

var_decl		:= IDENT [ LB CINT RB]

type		:= INT

​			| UNSIGNED [INT | LONG | SHORT | CHAR]

​			| CHAR

​			| FLOAT

​			| DOUBLE

​			| SHORT

​			| LONG

​			| VOID

parm_types	:=  type IDENT [LB RB] {COMMA type IDENT [LB RB]}

func 		:= type IDENT LP parm_types RP LC {type var_decl {COMMA var_decl} SEMI} {stmt} RC

stmt		:= IF LP expr RP stmt [ELSE stmt]

​			| WHILE LP expr RP stmt

​			| FOR LP [assg] SEMI [expr] SEMI [assg] RP stmt

​			| RETURN [expr] SEMI

​			| IDENT LP [expr{ COMMA expr}] RP SEMI # func call

​			| LC {stmt} RC

​			| assg SEMI

​			| BREAK SEMI

​			| CONTINUE SEMI

​			| SEMI

assg		:= IDENT [LB expr RB] ASSIGN expr

expr		:= cmp_expr {logical_op cmp_expr}

cmp_expr	:= arith_expr [relop arith_expr]

arith_expr	:= term {PLUS term}

​			| term {MINUS term}

term		:= factor {MULTIPLY factor}

​			| factor  {DIVIDE factor}

factor		:= IDENT [LP [expr { COMMA expr} ] RP | LB expr RB ]

​			| CINT | CUINT | CLONG | CULONG | CCHAR | CDOUBLE | CFLOAT	

​			| LP expr RP

relop		:= EQ

​			| NEQ

​			| LE

​			| LT

​			| GT

​			| GE

logical_op 	:= AND

​			| OR

​	 	