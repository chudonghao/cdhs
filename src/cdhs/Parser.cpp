//
// Created by chudonghao on 2022/2/1.
//

#include "Parser.h"
#include <iostream>

#include "CompileError.h"
#include "Lexer.h"
#include "ast/S.h"

namespace cdhs {

std::shared_ptr<ast::S> Parser::parse(Lexer &lexer) {
  m_lexer = &lexer;
  return S();
}

std::shared_ptr<ast::S> Parser::S() {
  auto S = std::make_shared<ast::S>();

  lexer().lex(1);
  auto &token = lexer().getToken(0);

  switch (token.kind) {
  case tok::eof:
    break;
  default:
    std::cerr << token.source << std::endl;
    throw CompileError(token.source_location);
  }

  return S;
}

} // namespace cdhs
