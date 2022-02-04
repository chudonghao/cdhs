## 文法定义

### token

* identifier : abc_123, etc.
* numeric_constant : 'c', 123, 0x123, 1.23, 1.23f, .123, .123E1, etc.
* string_literal : "fff"
* l_square: [
* r_square: ]
* l_paren: (
* r_paren: )
* l_brace: {
* r_brace: }
* period: .
* amp: &
* ampamp: &&
* star: *
* plus: +
* plusplus: ++
* minus: -
* arrow: ->
* minusminus: --
* tilde: ~
* exclaim: !
* exclaimequal: !=
* slash: /
* percent: %
* less: <
* lessequal: <=
* greater: >
* greaterequal: >=
* caret: ^
* pipe: |
* pipepipe: ||
* question: ?
* colon: :
* semi: ;
* equal: =
* equalequal: ==
* comma: ,

### 语法成分

* e 空
* TranslationUnitDecl 开始符号
* Decl 声明/定义
* TypedefDecl 别名声明
* FunctionDecl 函数声明
* Stmt 语句
* CompoundStmt 语句块
* DeclStmt 定义语句
* IfStmt IF语句
* ForStmt FOR语句
* ValueStmt 可能包含值的语句
* Expr 表达式
* DeclRefExpr 变量引用
* ParenExpr ()
* MemberExpr 成员访问
* ArraySubscriptExpr 数组成员访问
* ImplicitCastExpr 隐式转换
* CallExpr 函数调用
* UnaryOperator 一元操作符
* BinaryOperator 二元操作符
* ConditionalOperator ?:操作符
* IntegerLiteral 整型常量
* FloatingLiteral 浮点型常量
* StringLiteral 字符串常量

### 产生式

* TranslationUnitDecl ::= (Stmt ";") *

* Type ::= Id
* Type ::= "*" Type
* Type ::= "\[" { IntegerLiteral } "]" Type

* \ Decl ::= TypedefDecl
* \ Decl ::= FunctionDecl

* \ TypedefDecl ::= "typedef" Id Type ";"
* \ FunctionDecl ::= "func" Id "(" ParamVarDecl * ")" ";" | CompoundStmt

* CompoundStmt ::= "{" (Stmt ";") * "}"

* Stmt ::= IfStmt | ForStmt | ValueStmt
* ValueStmt ::= DeclStmt | Expr

* IfStmt ::= "if" "(" ValueStmt ")" CompoundStmt | e
* ForStmt ::= "for" "(" ValueStmt | e ";" ValueStmt | e ";" ValueStmt | e ")" CompoundStmt | e
* DeclStmt ::= "var" Id Type

* \ Expr ::= IntegerLiteral
* \ Expr ::= FloatingLiteral
* \ Expr ::= StringLiteral
* \ Expr ::= DeclRefExpr
* \ Expr ::= ParenExpr
* \ Expr ::= MemberExpr
* \ Expr ::= ArraySubscriptExpr
* \ Expr ::= ImplicitCastExpr
* \ Expr ::= UnaryOperator
* \ Expr ::= BinaryOperator
* \ Expr ::= ConditionalOperator

* Expr -> Term15

* Term -> Id <==== DeclRefExpr
* Term -> IntegerLiteral
* Term -> FloatingLiteral
* Term -> StringLiteral
* Term -> Id ( Term15 ) <==== CallExpr

* Term1 -> Term
* Term1 -> Term1.Id <==== MemberExpr
* Term1 -> Term1 -> Id <==== MemberExpr
* Term1 -> ( Term14 ) <==== ParenExpr
* Term1 -> Term1 [ Term14 ] <==== ArraySubscriptExpr

* Term2 -> Term1
* Term2 -> ! Term2
* Term2 -> - Term2
* Term2 -> ~ Term2
* Term2 -> ++ Term2
* Term2 -> -- Term2
* Term2 -> * Term2
* Term2 -> & Term2
* Term2 -> Term2 ++
* Term2 -> Term2 --

* Term3 -> Term2
* Term3 -> Term3 * Term3
* Term3 -> Term3 / Term3
* Term3 -> Term3 % Term3

* Term4 -> Term3
* Term4 -> Term4 + Term4
* Term4 -> Term4 - Term4

* Term5 -> Term4

* Term6 -> Term5
* Term6 -> Term6 > Term6
* Term6 -> Term6 >= Term6
* Term6 -> Term6 < Term6
* Term6 -> Term6 <= Term6

* Term7 -> Term6
* Term7 -> Term7 == Term7
* Term7 -> Term7 != Term7

* Term8 -> Term7
* Term8 -> Term8 & Term8

* Term9 -> Term8
* Term9 -> Term9 ^ Term9

* Term10 -> Term9
* Term10 -> Term10 | Term10

* Term11 -> Term10
* Term11 -> Term11 && Term11

* Term12 -> Term11
* Term12 -> Term12 || Term12

* Term13 -> Term12
* Term13 -> Term13 ? Term13 : Term13

* Term14 -> Term13
* Term14 -> Term14 = Term14

* Term15 -> Term14
* Term15 -> Term15 , Term15

### 产生式（提取公共因子）

* TranslationUnitDecl ::= (Stmt ";") *

* \ Decl ::= TypedefDecl
* \ Decl ::= FunctionDecl

* \ FunctionDecl ::= "func" Id "(" ParamVarDecl * ")" ";" | CompoundStmt

* CompoundStmt ::= "{" (Stmt ";") * "}"

* Stmt ::= IfStmt | ForStmt | ValueStmt
* ValueStmt ::= DeclStmt | Expr

* IfStmt ::= "if" "(" ValueStmt ")" CompoundStmt | e
* ForStmt ::= "for" "(" ValueStmt | e ";" ValueStmt | e ";" ValueStmt | e ")" CompoundStmt | e
* DeclStmt ::= "var" Id Type

* \ Expr ::= IntegerLiteral
* \ Expr ::= FloatingLiteral
* \ Expr ::= StringLiteral
* \ Expr ::= DeclRefExpr
* \ Expr ::= ParenExpr
* \ Expr ::= MemberExpr
* \ Expr ::= ArraySubscriptExpr
* \ Expr ::= ImplicitCastExpr
* \ Expr ::= UnaryOperator
* \ Expr ::= BinaryOperator
* \ Expr ::= ConditionalOperator

* Expr -> Term15

* Term -> Id Term_1
* Term -> IntegerLiteral
* Term -> FloatingLiteral
* Term -> StringLiteral
* Term_1 -> ( Term15 )
* Term_1 -> e

* Term1 -> Term
* Term1 -> Term1 Term1_1
* Term1 -> ( Term14 )
* Term1_1 -> . Id
* Term1_1 -> -> Id
* Term1_1 -> [ Term14 ]

* Term2 -> Term1
* Term2 -> ! Term2
* Term2 -> - Term2
* Term2 -> ~ Term2
* Term2 -> ++ Term2
* Term2 -> -- Term2
* Term2 -> * Term2
* Term2 -> & Term2
* Term2 -> Term2 Term2_1
* Term2_1 -> ++
* Term2_1 -> --

* Term3 -> Term2
* Term3 -> Term3 Term3_1
* Term3_1 -> * Term3
* Term3_1 -> / Term3
* Term3_1 -> % Term3

* Term4 -> Term3
* Term4 -> Term4 Term4_1
* Term4_1 -> + Term4
* Term4_1 -> - Term4

* Term5 -> Term4

* Term6 -> Term5
* Term6 -> Term6 Term6_1
* Term6_1 -> > Term6
* Term6_1 -> >= Term6
* Term6_1 -> < Term6
* Term6_1 -> <= Term6

* Term7 -> Term6
* Term7 -> Term7 Term7_1
* Term7_1 -> == Term7
* Term7_1 -> != Term7

* Term8 -> Term7
* Term8 -> Term8 & Term8

* Term9 -> Term8
* Term9 -> Term9 ^ Term9

* Term10 -> Term9
* Term10 -> Term10 | Term10

* Term11 -> Term10
* Term11 -> Term11 && Term11

* Term12 -> Term11
* Term12 -> Term12 || Term12

* Term13 -> Term12
* Term13 -> Term13 ? Term13 : Term13

* Term14 -> Term13
* Term14 -> Term14 = Term14

* Term15 -> Term14
* Term15 -> Term15 , Term15

### 产生式（消除左递归）

* TranslationUnitDecl ::= (Stmt ";") *

* \ Decl ::= TypedefDecl
* \ Decl ::= FunctionDecl

* \ FunctionDecl ::= "func" Id "(" ParamVarDecl * ")" ";" | CompoundStmt

* CompoundStmt ::= "{" (Stmt ";") * "}"

* Stmt ::= IfStmt | ForStmt | ValueStmt
* ValueStmt ::= DeclStmt | Expr

* IfStmt ::= "if" "(" ValueStmt ")" CompoundStmt | e
* ForStmt ::= "for" "(" ValueStmt | e ";" ValueStmt | e ";" ValueStmt | e ")" CompoundStmt | e
* DeclStmt ::= "var" Id Type

* \ Expr ::= IntegerLiteral
* \ Expr ::= FloatingLiteral
* \ Expr ::= StringLiteral
* \ Expr ::= DeclRefExpr
* \ Expr ::= ParenExpr
* \ Expr ::= MemberExpr
* \ Expr ::= ArraySubscriptExpr
* \ Expr ::= ImplicitCastExpr
* \ Expr ::= UnaryOperator
* \ Expr ::= BinaryOperator
* \ Expr ::= ConditionalOperator

* Expr -> Term15

* Term -> Id Term_1
* Term -> IntegerLiteral
* Term -> FloatingLiteral
* Term -> StringLiteral
* Term_1 -> ( Term15 )
* Term_1 -> e

* Term1_b -> Term
* Term1_b -> ( Term14 )
* Term1_a -> Term1_1
* Term1_1 -> . Id
* Term1_1 -> -> Id
* Term1_1 -> [ Term14 ]
* Term1 -> Term1_b Term1_A1
* Term1_A1 -> Term1_a Term1_A1
* Term1_A1 -> e

* Term2_b -> Term1
* Term2_b -> ! Term2
* Term2_b -> - Term2
* Term2_b -> ~ Term2
* Term2_b -> ++ Term2
* Term2_b -> -- Term2
* Term2_b -> * Term2
* Term2_b -> & Term2
* Term2_a -> ++
* Term2_a -> --
* Term2 -> Term2_b Term2_A1
* Term2_A1 -> Term2_a Term2_A1
* Term2_A1 -> e

* Term3_b -> Term2
* Term3_a -> * Term3
* Term3_a -> / Term3
* Term3_a -> % Term3
* Term3 -> Term3_b Term3_A1
* Term3_A1 -> Term3_a Term3_A1
* Term3_A1 -> e

* Term4_b -> Term3
* Term4_a -> + Term4
* Term4_a -> - Term4
* Term4 -> Term4_b Term4_A1
* Term4_A1 -> Term4_a Term4_A1
* Term4_A1 -> e

* Term5 -> Term4

* Term6_b -> Term5
* Term6_a -> > Term6
* Term6_a -> >= Term6
* Term6_a -> < Term6
* Term6_a -> <= Term6
* Term6 -> Term6_b Term6_A1
* Term6_A1 -> Term6_a Term6_A1
* Term6_A1 -> e

* Term7_b -> Term6
* Term7_a -> == Term7
* Term7_a -> != Term7
* Term7 -> Term7_b Term7_A1
* Term7_A1 -> Term7_a Term7_A1
* Term7_A1 -> e

* Term8_b -> Term7
* Term8_a -> & Term8
* Term8 -> Term8_b Term8_A1
* Term8_A1 -> Term8_a Term8_A1
* Term8_A1 -> e

* Term9_b -> Term8
* Term9_a -> ^ Term9
* Term9 -> Term9_b Term9_A1
* Term9_A1 -> Term9_a Term9_A1
* Term9_A1 -> e

* Term10_b -> Term9
* Term10_a -> | Term10
* Term10 -> Term10_b Term10_A1
* Term10_A1 -> Term10_a Term10_A1
* Term10_A1 -> e

* Term11_b -> Term10
* Term11_a -> && Term11
* Term11 -> Term11_b Term11_A1
* Term11_A1 -> Term11_a Term10_A1
* Term11_A1 -> e

* Term12_b -> Term11
* Term12_a -> || Term12
* Term12 -> Term12_b Term12_A1
* Term12_A1 -> Term12_a Term12_A1
* Term12_A1 -> e

* Term13_b -> Term12
* Term13_a -> ? Term13 : Term13
* Term13 -> Term13_b Term13_A1
* Term13_A1 -> Term13_a Term13_A1
* Term13_A1 -> e

* Term14_b -> Term13
* Term14_a -> = Term14
* Term14 -> Term14_b Term14_A1
* Term14_A1 -> Term14_a Term14_A1
* Term14_A1 -> e

* Term15_b -> Term14
* Term15_a -> , Term15
* Term15 -> Term15_b Term15_A1
* Term15_A1 -> Term15_a Term15_A1
* Term15_A1 -> e
