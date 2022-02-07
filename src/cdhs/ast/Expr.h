//
// Created by chudonghao on 2022/2/1.
//

#ifndef INC_2022_2_1_44186FE7DAD847159F4E4A7F4BDFEA0C_H
#define INC_2022_2_1_44186FE7DAD847159F4E4A7F4BDFEA0C_H

#include <memory>
#include <vector>

#include "../TokenKinds.h"
#include "ValueStmt.h"

namespace cdhs::ast {

/// Expr ::= IntegerLiteral
/// Expr ::= FloatingLiteral
/// Expr ::= StringLiteral
/// Expr ::= DeclRefExpr
/// Expr ::= ParenExpr
/// Expr ::= MemberExpr
/// Expr ::= ArraySubscriptExpr
/// Expr ::= ImplicitCastExpr
/// Expr ::= UnaryOperator
/// Expr ::= BinaryOperator
/// Expr ::= ConditionalOperator
///
/// Expr -> Term15
struct Expr : ValueStmt {};

struct IntegerLiteral : Expr {
  bool signed_{false};
  union {
    long long signed_value{};
    unsigned long long unsigned_value;
  };
};

struct FloatingLiteral : Expr {
  double value;
};

struct StringLiteral : Expr {
  std::string str;
};

// 临时的，最终会称为CallExpr中的args
struct CallArgsExpr : Expr {
  std::vector<std::unique_ptr<Expr>> args;
};

struct CallExpr : Expr {
  std::string id;
  std::vector<std::unique_ptr<Expr>> args;
};

struct ArraySubscriptExpr : Expr {
  std::unique_ptr<Expr> array;
  std::unique_ptr<Expr> subscript;
};

struct DeclRefExpr : Expr {
  std::string id;
};

struct UnaryOperator : Expr {
  enum Side { LHS, RHS };
  tok::TokenKind op;
  std::unique_ptr<Expr> expr;
  Side side /*op side*/;
};

struct BinaryOperator : Expr {
  std::unique_ptr<Expr> l_expr;
  tok::TokenKind op;
  std::unique_ptr<Expr> r_expr;
};

struct ConditionalOperator : Expr {
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Expr> true_expr;
  std::unique_ptr<Expr> false_expr;
};

} // namespace cdhs::ast

#endif // INC_2022_2_1_44186FE7DAD847159F4E4A7F4BDFEA0C_H
