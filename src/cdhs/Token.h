//
// Created by chudonghao on 2022/1/28.
//

#ifndef CDHS_TOKEN_H
#define CDHS_TOKEN_H

#include <string>

#include "SourceLocation.h"
#include "TokenKinds.h"

namespace cdhs {

class Token {
public:
  SourceLocation source_location{};
  std::string source;
  tok::TokenKind kind{};

  Token() = default;
  Token(const SourceLocation &source_location, std::string source, tok::TokenKind kind);
  Token(const SourceLocation &source_location, tok::TokenKind kind);
};

} // namespace cdhs

#endif // CDHS_TOKEN_H
