## 文法定义

### token

```c++
enum TokenKind {
  unknown,          //
  comment,          //
                    //
  extern_,          // extern
  typedef_,         // typedef
  if_,              // if
  else_,            // else
  for_,             // for
  continue_,        // continue
  break_,           // break
  syntax,           // syntax
  func,             // func
  var,              // var
                    //
  identifier,       // abc_123, etc.
  char_constant,    // 'c'
  numeric_constant, // 123, 0x123, 1.23, 1.23f, .123, .123E1, etc.
  string_literal,   // "fff"
  l_square,         // [
  r_square,         // ]
  l_paren,          // (
  r_paren,          // )
  l_brace,          // {
  r_brace,          // }
  period,           // .
  amp,              // &
  ampamp,           // &&
  star,             // *
  plus,             // +
  plusplus,         // ++
  minus,            // -
  arrow,            // ->
  minusminus,       // --
  tilde,            // ~
  exclaim,          // !
  exclaimequal,     // !=
  slash,            // /
  percent,          // %
  less,             // <
  lessequal,        // <=
  greater,          // >
  greaterequal,     // >=
  caret,            // ^
  pipe,             // |
  pipepipe,         // ||
  question,         // ?
  colon,            // :
  semi,             // ;
  equal,            // =
  equalequal,       // ==
  comma,            // ,
  blank,            // space \t
  eol,              // end of line \r\n \r \n
  eof,              // end of file

  tok_max,
};
```

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

顶层文法分析并不复杂，采用递归下降分析，定义如下：

* TranslationUnitDecl ::= Stmt *

* Type ::= Id
* Type ::= "*" Type
* Type ::= "\[" { IntegerLiteral } "]" Type

* \ Decl ::= TypedefDecl
* \ Decl ::= FunctionDecl
* \ Decl ::= VarDecl

* \ VarDecl ::= "var" Id Type ";"
* \ TypedefDecl ::= "typedef" Id Type ";"
* \ FunctionDecl ::= "func" Id "(" ParamVarDecl * ")" ";" | CompoundStmt

* CompoundStmt ::= "{" Stmt * "}"

* Stmt ::= IfStmt
* Stmt ::= ForStmt
* Stmt ::= ValueStmt
* Stmt ::= NullStmt
* ValueStmt ::= DeclStmt
* ValueStmt ::= Expr { ";" }
* NullStmt ::=

* IfStmt ::= "if" "(" ValueStmt ")" CompoundStmt | Stmt { "else" CompoundStmt | ";" }
* ForStmt ::= "for" "(" ValueStmt | NullStmt ValueStmt | NullStmt ValueStmt | NullStmt)" CompoundStmt | ";"
* DeclStmt ::= "var" Id Type { "=" Expr } { ";" }

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

分号比较特殊，我们通过函数参数来决定是否必要（特别DeclStmt和ValueStmt，因为它们会位于if和for的()中）

Expr包含运算符分析（优先级、结合性），相对复杂，我们采用SLR(1)文法：

* Expr -> Term15

* StringLiterals -> string_literal
* StringLiterals -> StringLiterals string_literal
* FuncArgs -> Term14
* FuncArgs -> FuncArgs , Term14

* Term -> identifier
* Term -> char_constant
* Term -> numeric_constant
* Term -> StringLiterals
* Term -> identifier ( FuncArgs )
* Term -> identifier ( )

* Term1 -> Term
* Term1 -> Term1 . identifier
* Term1 -> Term1 -> identifier
* Term1 -> ( Term14 )
* Term1 -> Term1 [ Term14 ]

* Term2 -> Term1
* Term2 -> ! Term2
* Term2 -> - Term2
* Term2 -> ~ Term2
* Term2 -> ++ Term2
* Term2 -> -- Term2
* Term2 -> * Term2
* Term2 -> & Term2
* Term2 -> Term1 ++
* Term2 -> Term1 --

* Term3 -> Term2
* Term3 -> Term3 * Term2
* Term3 -> Term3 / Term2
* Term3 -> Term3 % Term2

* Term4 -> Term3
* Term4 -> Term4 + Term3
* Term4 -> Term4 - Term3

* Term5 -> Term4

* Term6 -> Term5
* Term6 -> Term6 > Term5
* Term6 -> Term6 >= Term5
* Term6 -> Term6 < Term5
* Term6 -> Term6 <= Term5

* Term7 -> Term6
* Term7 -> Term7 == Term6
* Term7 -> Term7 != Term6

* Term8 -> Term7
* Term8 -> Term8 & Term7

* Term9 -> Term8
* Term9 -> Term9 ^ Term8

* Term10 -> Term9
* Term10 -> Term10 | Term9

* Term11 -> Term10
* Term11 -> Term11 && Term10

* Term12 -> Term11
* Term12 -> Term12 || Term11

* Term13 -> Term12
* Term13 -> Term12 ? Term13 : Term13

* Term14 -> Term13
* Term14 -> Term13 = Term14

* Term15 -> Term14
* Term15 -> Term15 , Term14
