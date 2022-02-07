//
// Created by chudonghao on 2022/2/1.
//

#ifndef CDHS_PARSER_H
#define CDHS_PARSER_H

#include "ExprParser.h"
#include <memory>

namespace cdhs {

namespace ast {
struct TranslationUnitDecl;
struct Stmt;
struct IfStmt;
struct ForStmt;
struct DeclStmt;
struct ValueStmt;
struct Expr;
} // namespace ast

class Lexer;
class Parser {
public:
  std::unique_ptr<ast::TranslationUnitDecl> parse(Lexer &lexer);

private:
  std::unique_ptr<ast::TranslationUnitDecl> TranslationUnitDecl();
  std::unique_ptr<ast::Stmt> Stmt();
  std::unique_ptr<ast::IfStmt> IfStmt();
  std::unique_ptr<ast::ForStmt> ForStmt();
  std::unique_ptr<ast::ValueStmt> ValueStmt(bool eat_semi);
  std::unique_ptr<ast::DeclStmt> DeclStmt(bool eat_semi);
  std::unique_ptr<ast::Expr> Expr(bool eat_semi);

private:
  Lexer &lexer() { return *m_lexer; }

  Lexer *m_lexer{};

  ExprParser m_expr_parser;
};

} // namespace cdhs

#endif // CDHS_PARSER_H
