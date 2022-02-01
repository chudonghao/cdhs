//
// Created by chudonghao on 2022/1/28.
//

#ifndef CDHS_TOKENKINDS_H
#define CDHS_TOKENKINDS_H

#include <string>

namespace cdhs::tok {

enum TokenKind {
  unknown,          //
  comment,          //
  identifier,       // abc_123, etc.
  char_constant,    // 'c'
  numeric_constant, // 123, 0x123, 1.23, 1.23f, .123, .123E1, etc.
  string_literal,   // "fff"
  l_square,         // [
  r_square,         // ]
  l_paren,          // (
  r_paren,          // )
  l_brace,          // {
  r_brace,          // }
  period,           // .
  amp,              // &
  ampamp,           // &&
  star,             // *
  plus,             // +
  plusplus,         // ++
  minus,            // -
  arrow,            // ->
  minusminus,       // --
  tilde,            // ~
  exclaim,          // !
  exclaimequal,     // !=
  slash,            // /
  percent,          // %
  less,             // <
  lessequal,        // <=
  greater,          // >
  greaterequal,     // >=
  caret,            // ^
  pipe,             // |
  pipepipe,         // ||
  question,         // ?
  colon,            // :
  semi,             // ;
  equal,            // =
  equalequal,       // ==
  comma,            // ,
  blank,            // space \t
  eol,              // end of line \r\n \r \n
  eof,              // end of file
  tok_max = 1024,
};

std::string name(TokenKind kind);

} // namespace cdhs

#endif // CDHS_TOKENKINDS_H
