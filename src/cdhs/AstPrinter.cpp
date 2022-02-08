//
// Created by chudonghao on 2022/2/7.
//

#include "AstPrinter.h"

#include <iostream>

#include "ast/DeclStmt.h"
#include "ast/Expr.h"
#include "ast/ForStmt.h"
#include "ast/IfStmt.h"
#include "ast/Stmt.h"
#include "ast/Term.h"
#include "ast/TranslationUnitDecl.h"
#include "ast/ValueStmt.h"

namespace cdhs {

void AstPrinter::print(const ast::TranslationUnitDecl &decl, int indent) {
  std::string indent_(indent * 4, ' ');
  std::string indent2_(indent * 4 + 4, ' ');
  std::cout << indent_ << "TranslationUnitDecl" << std::endl;
  for (const auto &stmt : decl.Stmts) {
    if (auto expr = dynamic_cast<const ast::Expr *>(stmt.get())) {
      print(*expr, indent + 1);
      continue;
    }
    std::cout << indent2_ << "Unknown" << std::endl;
  }
}
void AstPrinter::print(const ast::DeclRefExpr &expr, int indent) {
  std::string indent_(indent * 4, ' ');
  std::string indent2_(indent * 4 + 4, ' ');
  std::cout << indent_ << "DeclRefExpr " << expr.id << std::endl;
}

void AstPrinter::print(const ast::ArraySubscriptExpr &expr, int indent) {
  std::string indent_(indent * 4, ' ');
  std::string indent2_(indent * 4 + 4, ' ');
  std::cout << indent_ << "ArraySubscriptExpr" << std::endl;
  if (expr.array) {
    print(*expr.array, indent + 1);
  } else {
    std::cout << indent2_ << "Null" << std::endl;
  }

  if (expr.subscript) {
    print(*expr.subscript, indent + 1);
  } else {
    std::cout << indent2_ << "Null" << std::endl;
  }
}

void AstPrinter::print(const ast::CallExpr &expr, int indent) {
  std::string indent_(indent * 4, ' ');
  std::string indent2_(indent * 4 + 4, ' ');
  std::cout << indent_ << "CallExpr " << expr.id << std::endl;
  if (!expr.args.empty()) {
    for (const auto &arg : expr.args) {
      print(*arg, indent + 1);
    }
  } else {
    std::cout << indent2_ << "Null" << std::endl;
  }
}

void AstPrinter::print(const ast::UnaryOperator &expr, int indent) {
  std::string indent_(indent * 4, ' ');
  std::string indent2_(indent * 4 + 4, ' ');
  std::cout << indent_ << "UnaryOperator " << tok::to_kind_name(expr.op) << " ";
  if (expr.side == ast::UnaryOperator::LHS) {
    std::cout << "LHS";
  } else {
    std::cout << "RHS";
  }
  std::cout << std::endl;
  print(*expr.expr, indent + 1);
}

void AstPrinter::print(const ast::BinaryOperator &expr, int indent) {
  std::string indent_(indent * 4, ' ');
  std::string indent2_(indent * 4 + 4, ' ');
  std::cout << indent_ << "BinaryOperator " << tok::to_kind_name(expr.op) << std::endl;
  print(*expr.l_expr, indent + 1);
  print(*expr.r_expr, indent + 1);
}

void AstPrinter::print(const ast::ConditionalOperator &expr, int indent) {
  std::string indent_(indent * 4, ' ');
  std::string indent2_(indent * 4 + 4, ' ');
  std::cout << indent_ << "ConditionalOperator" << std::endl;
  print(*expr.condition, indent + 1);
  print(*expr.true_expr, indent + 1);
  print(*expr.false_expr, indent + 1);
}

void AstPrinter::print(const ast::IntegerLiteral &expr, int indent) {
  std::string indent_(indent * 4, ' ');
  std::string indent2_(indent * 4 + 4, ' ');
  std::cout << indent_ << "IntegerLiteral " << expr.unsigned_value << std::endl;
}

void AstPrinter::print(const ast::FloatingLiteral &expr, int indent) {
  std::string indent_(indent * 4, ' ');
  std::string indent2_(indent * 4 + 4, ' ');
  std::cout << indent_ << "FloatingLiteral " << expr.value << std::endl;
}

void AstPrinter::print(const ast::StringLiteral &expr, int indent) {
  std::string indent_(indent * 4, ' ');
  std::string indent2_(indent * 4 + 4, ' ');
  std::cout << indent_ << "StringLiteral " << expr.str << std::endl;
}

void AstPrinter::print(const ast::Expr &expr, int indent) {
  if (auto s = dynamic_cast<const ast::DeclRefExpr *>(&expr)) {
    print(*s, indent);
    return;
  }
  if (auto s = dynamic_cast<const ast::ArraySubscriptExpr *>(&expr)) {
    print(*s, indent);
    return;
  }
  if (auto s = dynamic_cast<const ast::CallExpr *>(&expr)) {
    print(*s, indent);
    return;
  }
  if (auto s = dynamic_cast<const ast::UnaryOperator *>(&expr)) {
    print(*s, indent);
    return;
  }
  if (auto s = dynamic_cast<const ast::BinaryOperator *>(&expr)) {
    print(*s, indent);
    return;
  }
  if (auto s = dynamic_cast<const ast::ConditionalOperator *>(&expr)) {
    print(*s, indent);
    return;
  }
  if (auto s = dynamic_cast<const ast::IntegerLiteral *>(&expr)) {
    print(*s, indent);
    return;
  }
  if (auto s = dynamic_cast<const ast::FloatingLiteral *>(&expr)) {
    print(*s, indent);
    return;
  }
  if (auto s = dynamic_cast<const ast::StringLiteral *>(&expr)) {
    print(*s, indent);
    return;
  }
  std::cout << std::string(indent * 4, ' ') << "Unknown" << std::endl;
}

} // namespace cdhs