//
// Created by chudonghao on 2022/2/7.
//

#ifndef INC_2022_2_7_16D8D11FE2BA4665AC58CF235991CD4E_H
#define INC_2022_2_7_16D8D11FE2BA4665AC58CF235991CD4E_H

#include "ast/DeclStmt.h"
#include "ast/Expr.h"
#include "ast/ForStmt.h"
#include "ast/IfStmt.h"
#include "ast/Stmt.h"
#include "ast/Term.h"
#include "ast/TranslationUnitDecl.h"
#include "ast/ValueStmt.h"

namespace cdhs {

class AstPrinter {
public:
  /// TODO 完善更多类型

  void print(const ast::TranslationUnitDecl &decl, int indent = 0);
  void print(const ast::DeclRefExpr &expr, int indent);
  void print(const ast::ArraySubscriptExpr &expr, int indent);
  void print(const ast::CallExpr &expr, int indent);
  void print(const ast::UnaryOperator &expr, int indent);
  void print(const ast::BinaryOperator &expr, int indent);
  void print(const ast::ConditionalOperator &expr, int indent);
  void print(const ast::IntegerLiteral &expr, int indent);
  void print(const ast::FloatingLiteral &expr, int indent);
  void print(const ast::StringLiteral &expr, int indent);

private:
  void print(const ast::Expr &expr, int indent);
};

} // namespace cdhs

#endif // INC_2022_2_7_16D8D11FE2BA4665AC58CF235991CD4E_H
