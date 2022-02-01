//
// Created by chudonghao on 2022/1/28.
//

#ifndef CDHS_LEXER_H
#define CDHS_LEXER_H

#include <deque>
#include <string>

#include "SourceLocation.h"
#include "Token.h"

namespace cdhs {
class Lexer {
public:
  Lexer();
  ~Lexer();

  int lex(size_t n_token);
  int advance(size_t n_token);
  int advanceAndLex(size_t advance_n_token, size_t lex_n_token = 1);

  void reset(std::string *source);

  int getTokenCount() const;
  const Token &getToken(size_t index) const;

private:
  int lexOneToken();

  void *m_internal_lexer{};

  std::deque<Token> m_tokens;
};
} // namespace cdhs

#endif // CDHS_LEXER_H
