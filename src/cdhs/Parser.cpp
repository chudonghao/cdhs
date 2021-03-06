//
// Created by chudonghao on 2022/2/1.
//

#include "Parser.h"

#include <iostream>

#include "CompileError.h"
#include "ExprParser.h"
#include "Lexer.h"
#include "ast/DeclStmt.h"
#include "ast/Expr.h"
#include "ast/ForStmt.h"
#include "ast/IfStmt.h"
#include "ast/TranslationUnitDecl.h"
#include "ast/ValueStmt.h"

namespace cdhs {

std::unique_ptr<ast::TranslationUnitDecl> Parser::parse(Lexer &lexer) {
  m_lexer = &lexer;
  m_expr_parser.m_lexer = m_lexer;
  return TranslationUnitDecl();
}

std::unique_ptr<ast::TranslationUnitDecl> Parser::TranslationUnitDecl() {
  // TranslationUnitDecl ::= (Stmt ";") *
  auto TranslationUnitDecl = std::make_unique<ast::TranslationUnitDecl>();

  for (;;) {
    lexer().lex(1);
    auto &token = lexer().getToken(0);

    // $
    if (token.kind == tok::eof) {
      return TranslationUnitDecl;
    }

    // any kind
    auto Stmt = this->Stmt();
    TranslationUnitDecl->Stmts.emplace_back(std::move(Stmt));
  }
}

std::unique_ptr<ast::Stmt> Parser::Stmt() {
  // Stmt ::= IfStmt | ForStmt | ValueStmt
  // TODO 还没有精确确定各个SELECT集

  lexer().lex(1);
  auto &token = lexer().getToken(0);

  if (token.kind == tok::if_) {
    throw CompileError("not support if yet");
    return IfStmt();
  }

  if (token.kind == tok::for_) {
    throw CompileError("not support for yet");
    return ForStmt();
  }

  { return ValueStmt(true); }

  throw CompileError(token.source_location);
}

std::unique_ptr<ast::IfStmt> Parser::IfStmt() { return std::unique_ptr<ast::IfStmt>(); }

std::unique_ptr<ast::ForStmt> Parser::ForStmt() { return std::unique_ptr<ast::ForStmt>(); }

std::unique_ptr<ast::ValueStmt> Parser::ValueStmt(bool eat_semi) {
  // ValueStmt ::= DeclStmt | Expr
  // SELECT(ValueStmt :: = DeclStmt) = "var"
  // SELECT(ValueStmt ::= Expr) = FIRST(Expr)
  //     ! & ( * ++ - -- FloatingLiteral Id IntegerLiteral StringLiteral ~

  lexer().lex(1);
  auto &token = lexer().getToken(0);

  if (token.kind == tok::var) {
    return DeclStmt(eat_semi);
  }

  switch (token.kind) {
  case tok::exclaim:
  case tok::amp:
  case tok::l_paren:
  case tok::star:
  case tok::plusplus:
  case tok::minus:
  case tok::minusminus:
  case tok::numeric_constant:
  case tok::char_constant:
  case tok::identifier:
  case tok::string_literal:
  case tok::tilde:
    return Expr(eat_semi);
  default:
    throw CompileError(token.source_location, "expect ! & ( * ++ - -- FloatingLiteral Id IntegerLiteral StringLiteral ~");
  }
}

std::unique_ptr<ast::DeclStmt> Parser::DeclStmt(bool eat_semi) { return std::unique_ptr<ast::DeclStmt>(); }

std::unique_ptr<ast::Expr> Parser::Expr(bool eat_semi) {
  auto expr = m_expr_parser.Expr();

  // eat ;
  if (eat_semi) {
    lexer().lex(1);
    auto token = lexer().getToken(0);

    if (token.kind != tok::semi) {
      throw CompileError(token.source_location, "expect ;");
    }

    lexer().advance(1);
  }
  return expr;
}

} // namespace cdhs
