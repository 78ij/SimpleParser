##  The Syntax of the C-like language : EBNF



prog		 := {dcl SEMI | func}

dcl			 := [EXTERN] type var_decl { COMMA var_decl}

​			| [EXTERN] type IDENT LP parm_types RP {COMMA IDENT LP parm_types RP }

​			| [EXTERN] VOID IDENT LP parm_types RP {COMMA IDENT LP parm_types RP }

var_decl		:= IDENT [ LB CINT RB]

type		:= INT

​			| UNSIGNED [INT | LONG | SHORT | CHAR]

​			| CHAR

​			| FLOAT

​			| DOUBLE

​			| SHORT

​			| LONG

parm_types	:= VOID

​			| type IDENT [LB RB] {COMMA type IDENT [LB RB]}

func 		:= type IDENT LP parm_types RP LC {type var_decl {COMMA var_decl} SEMI} {stmt} RC

​			|  VOID IDENT LP parm_types RP LC {type var_decl {COMMA var_decl} SEMI} {stmt} RC #func def

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

expr		:= MINUS expr

​			| PLUS expr

​			| NOT expr

​			| expr binop expr

​			| expr relop expr

​			| expr logical_op expr

​			| IDENT [LP [expr { COMMA expr} ] RP | LB expr RB ]

​			| LP expr RP

​			| CINT

​			 

binop		:= PLUS

​			| MINUS

​			| MULTIPLY

​			| DIVIDE

relop		:= EQ

​			| NEQ

​			| LE

​			| LT

​			| GT

​			| GE

logical_op 	:= AND

​			| OR

​	 	