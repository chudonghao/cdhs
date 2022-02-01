//
// Created by chudonghao on 2022/1/28.
//

#include "Token.h"

#include <utility>
namespace cdhs {

Token::Token(const SourceLocation &source_location, std::string source, tok::TokenKind kind)
    : source_location(source_location), source(std::move(source)), kind(kind) {}

Token::Token(const SourceLocation &source_location, tok::TokenKind kind) : source_location(source_location), source(), kind(kind) {}

} // namespace cdhs
