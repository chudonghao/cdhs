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
* e （空）
* S（开始符号）
* Sentence（句子）
* Expr（表达式）
* DeclareType 声明类型
* DeclareVar 声明变量
* DeclareFunc 声明函数
* DefineType 定义类型
* DefineVar 变量变量
* DefineFunc 定义函数
* Id 标识符

### 产生式

Token -> Id | ConstNumber

* S -> e
* S -> Primary S

* Primary -> Sentence

* Sentence -> Expr ;
* Sentence -> if ( Expr ) { Sentences }
* Sentence -> if ( Expr ) Sentence
* Sentence -> for ( ; Expr ; Expr) { Sentences }
* Sentence -> for ( ; Expr ; Expr) Sentence

* Sentences -> e
* Sentences -> Sentence Sentences

* Expr -> Term15

* Term -> Id
* Term -> ConstNumber 
* Term -> Id ( Term15 )

* Term1 -> Term
* Term1 -> Term1.Id
* Term1 -> Term1 -> Id
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

* S -> e
* S -> Primary S

* Primary -> Sentence

* Sentence -> Expr ;
* Sentence -> if ( Expr ) Sentence_1
* Sentence -> for ( ; Expr ; Expr) Sentence_1
* Sentence_1 -> { Sentences }
* Sentence_1 -> Sentence

* Sentences -> e
* Sentences -> Sentence Sentences

* Expr -> Term15

* Term -> Id Term_1
* Term -> ConstNumber
* Term_1 -> e
* Term_1 -> ( Term15 )

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
* Term8 -> Term8 Term8_1
* Term8_1 -> & Term8

* Term9 -> Term8
* Term9 -> Term9 Term9_1
* Term9_1 -> ^ Term9

* Term10 -> Term9
* Term10 -> Term10 Term10_1
* Term10_1 -> | Term10

* Term11 -> Term10
* Term11 -> Term11 Term11_1
* Term11_1 -> && Term11

* Term12 -> Term11
* Term12 -> Term12 Term12_1
* Term12_1 -> || Term12

* Term13 -> Term12
* Term13 -> Term13 Term13_1
* Term13_1 -> ? Term13 : Term13

* Term14 -> Term13
* Term14 -> Term14 Term14_1
* Term14_1 -> = Term14

* Term15 -> Term14
* Term15 -> Term15 Term15_1
* Term15_1 -> , Term15

### 产生式（消除左递归）

* S -> e
* S -> Primary S

* Primary -> Sentence

* Sentence -> Expr ;
* Sentence -> if ( Expr ) Sentence_1
* Sentence -> for ( ; Expr ; Expr) Sentence_1
* Sentence_1 -> { Sentences }
* Sentence_1 -> Sentence

* Sentences -> e
* Sentences -> Sentence Sentences

* Expr -> Term15

* Term -> Id Term_1
* Term -> ConstNumber
* Term_1 -> e
* Term_1 -> ( Term15 )

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

| 产生式                                         | FIRST | FOLLOW | SELECT |
|---------------------------------------------|------|--------|--------|
| S -> e                                      ||||
| S -> Primary S                              ||||
| Primary -> Sentence                         ||||
| Sentence -> Expr ;                          ||||
| Sentence -> if ( Expr ) Sentence_1          ||||
| Sentence -> for ( ; Expr ; Expr) Sentence_1 ||||
| Sentence_1 -> { Sentences }                 ||||
| Sentence_1 -> Sentence                      ||||
| Sentences -> e                              ||||
| Sentences -> Sentence Sentences             ||||
| Expr -> Term15                              ||||
| Term -> Id Term_1                           ||||
| Term -> ConstNumber                         ||||
| Term_1 -> e                                 ||||
| Term_1 -> ( Term15 )                        ||||
| Term1_b -> Term                             ||||
| Term1_b -> ( Term14 )                       ||||
| Term1_a -> Term1_1                          ||||
| Term1_1 -> . Id                             ||||
| Term1_1 -> -> Id                            ||||
| Term1_1 -> [ Term14 ]                       ||||
| Term1 -> Term1_b Term1_A1                   ||||
| Term1_A1 -> Term1_a Term1_A1                ||||
| Term1_A1 -> e                               ||||
| Term2_b -> Term1                            ||||
| Term2_b -> ! Term2                          ||||
| Term2_b -> - Term2                          ||||
| Term2_b -> ~ Term2                          ||||
| Term2_b -> ++ Term2                         ||||
| Term2_b -> -- Term2                         ||||
| Term2_b -> * Term2                          ||||
| Term2_b -> & Term2                          ||||
| Term2_a -> ++                               ||||
| Term2_a -> --                               ||||
| Term2 -> Term2_b Term2_A1                   ||||
| Term2_A1 -> Term2_a Term2_A1                ||||
| Term2_A1 -> e                               ||||
| Term3_b -> Term2                            ||||
| Term3_a -> * Term3                          ||||
| Term3_a -> / Term3                          ||||
| Term3_a -> % Term3                          ||||
| Term3 -> Term3_b Term3_A1                   ||||
| Term3_A1 -> Term3_a Term3_A1                ||||
| Term3_A1 -> e                               ||||
| Term4_b -> Term3                            ||||
| Term4_a -> + Term4                          ||||
| Term4_a -> - Term4                          ||||
| Term4 -> Term4_b Term4_A1                   ||||
| Term4_A1 -> Term4_a Term4_A1                ||||
| Term4_A1 -> e                               ||||
| Term5 -> Term4                              ||||
| Term6_b -> Term5                            ||||
| Term6_a -> > Term6                          ||||
| Term6_a -> >= Term6                         ||||
| Term6_a -> < Term6                          ||||
| Term6_a -> <= Term6                         ||||
| Term6 -> Term6_b Term6_A1                   ||||
| Term6_A1 -> Term6_a Term6_A1                ||||
| Term6_A1 -> e                               ||||
| Term7_b -> Term6                            ||||
| Term7_a -> == Term7                         ||||
| Term7_a -> != Term7                         ||||
| Term7 -> Term7_b Term7_A1                   ||||
| Term7_A1 -> Term7_a Term7_A1                ||||
| Term7_A1 -> e                               ||||
| Term8_b -> Term7                            ||||
| Term8_a -> & Term8                          ||||
| Term8 -> Term8_b Term8_A1                   ||||
| Term8_A1 -> Term8_a Term8_A1                ||||
| Term8_A1 -> e                               ||||
| Term9_b -> Term8                            ||||
| Term9_a -> ^ Term9                          ||||
| Term9 -> Term9_b Term9_A1                   ||||
| Term9_A1 -> Term9_a Term9_A1                ||||
| Term9_A1 -> e                               ||||
| Term10_b -> Term9                           ||||
| Term10_a -> &#124; Term10                   ||||
| Term10 -> Term10_b Term10_A1                ||||
| Term10_A1 -> Term10_a Term10_A1             ||||
| Term10_A1 -> e                              ||||
| Term11_b -> Term10                          ||||
| Term11_a -> && Term11                       ||||
| Term11 -> Term11_b Term11_A1                ||||
| Term11_A1 -> Term11_a Term10_A1             ||||
| Term11_A1 -> e                              ||||
| Term12_b -> Term11                          ||||
| Term12_a -> &#124;&#124; Term12             ||||
| Term12 -> Term12_b Term12_A1                ||||
| Term12_A1 -> Term12_a Term12_A1             ||||
| Term12_A1 -> e                              ||||
| Term13_b -> Term12                          ||||
| Term13_a -> ? Term13 : Term13               ||||
| Term13 -> Term13_b Term13_A1                ||||
| Term13_A1 -> Term13_a Term13_A1             ||||
| Term13_A1 -> e                              ||||
| Term14_b -> Term13                          ||||
| Term14_a -> = Term14                        ||||
| Term14 -> Term14_b Term14_A1                ||||
| Term14_A1 -> Term14_a Term14_A1             ||||
| Term14_A1 -> e                              ||||
| Term15_b -> Term14                          ||||
| Term15_a -> , Term15                        ||||
| Term15 -> Term15_b Term15_A1                ||||
| Term15_A1 -> Term15_a Term15_A1             ||||
| Term15_A1 -> e                              ||||
