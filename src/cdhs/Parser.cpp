//
// Created by chudonghao on 2022/2/1.
//

#include "Parser.h"
#include <iostream>

#include "CompileError.h"
#include "Lexer.h"
#include "ast/Primary.h"
#include "ast/S.h"
#include "ast/Sentence.h"

namespace cdhs {

std::shared_ptr<ast::S> Parser::parse(Lexer &lexer) {
  m_lexer = &lexer;
  return S();
}

std::shared_ptr<ast::S> Parser::S() {
  /// SELECT(S -> e)         <==== $
  /// SELECT(S -> Primary S) <==== ! & ( * ++ - -- ConstNumber Id for if ~

  auto S = std::make_shared<ast::S>();

  lexer().lex(1);
  auto &token = lexer().getToken(0);

  switch (token.kind) {
  case tok::eof: {
    S->select = ast::S::SELECT_e;
    break;
  }
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
  case tok::for_:
  case tok::if_:
  case tok::tilde: {
    S->select = ast::S::SELECT_Primary_S;
    S->Primary = this->Primary();
    S->S = this->S();
    break;
  }
  default:
    throw CompileError(token.source_location);
  }

  return S;
}

std::shared_ptr<ast::Primary> Parser::Primary() {
  /// SELECT(Primary -> Sentence) <==== ! & ( * ++ - -- ConstNumber Id for if ~

  auto Primary = std::make_shared<ast::Primary>();
  Primary->Sentence = this->Sentence();
  return Primary;
}

std::shared_ptr<ast::Sentence> Parser::Sentence() {
  /// SELECT(Sentence -> Expr ;)                          <==== ! & ( * ++ - -- ConstNumber Id ~
  /// SELECT(Sentence -> if ( Expr ) Sentence_1)          <==== if
  /// SELECT(Sentence -> for ( ; Expr ; Expr) Sentence_1) <==== for

  auto Sentence = std::make_shared<ast::Sentence>();

  lexer().lex(1);
  auto &token = lexer().getToken(0);

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
  case tok::tilde: {
    break;
  }
  case tok::for_:
    break;
  case tok::if_:
    break;
  default:
    throw CompileError(token.source_location);
  }

  return Sentence;
}

} // namespace cdhs
