//
// Created by chudonghao on 2022/2/1.
//

#ifndef CDHS_PARSER_H
#define CDHS_PARSER_H

#include <memory>

namespace cdhs {

namespace ast {
struct S;
struct Primary;
struct Sentence;
} // namespace ast

class Lexer;
class Parser {
public:
  std::shared_ptr<ast::S> parse(Lexer &lexer);

private:
  std::shared_ptr<ast::S> S();
  std::shared_ptr<ast::Primary> Primary();
  std::shared_ptr<ast::Sentence> Sentence();

private:
  Lexer &lexer() { return *m_lexer; }

  Lexer *m_lexer{};
};

} // namespace cdhs

#endif // CDHS_PARSER_H
