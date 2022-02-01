//
// Created by chudonghao on 2022/2/1.
//

#ifndef CDHS_PARSER_H
#define CDHS_PARSER_H

#include <memory>

namespace cdhs {
namespace ast {
class S;
}
class Lexer;
class Parser {
public:
  std::shared_ptr<ast::S> parse(Lexer &lexer);

private:
  std::shared_ptr<ast::S> S();

private:
  Lexer &lexer() { return *m_lexer; }

  Lexer *m_lexer{};
};

} // namespace cdhs

#endif // CDHS_PARSER_H
