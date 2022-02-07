//
// Created by chudonghao on 2022/1/28.
//

#include "TokenKinds.h"

namespace cdhs::tok {

std::string to_kind_name(cdhs::tok::TokenKind kind) {
  switch (kind) {
  case unknown:
    return "unknown";
  case comment:
    return "comment";
  case extern_:
    return "extern";
  case typedef_:
    return "typedef";
  case if_:
    return "if";
  case else_:
    return "else";
  case for_:
    return "for";
  case continue_:
    return "continue";
  case break_:
    return "break";
  case syntax:
    return "syntax";
  case func:
    return "func";
  case var:
    return "var";
  case identifier:
    return "identifier";
  case char_constant:
    return "char_constant";
  case numeric_constant:
    return "numeric_constant";
  case string_literal:
    return "string_literal";
  case l_square:
    return "l_square";
  case r_square:
    return "r_square";
  case l_paren:
    return "l_paren";
  case r_paren:
    return "r_paren";
  case l_brace:
    return "l_brace";
  case r_brace:
    return "r_brace";
  case period:
    return "period";
  case amp:
    return "amp";
  case ampamp:
    return "ampamp";
  case star:
    return "star";
  case plus:
    return "plus";
  case plusplus:
    return "plusplus";
  case minus:
    return "minus";
  case arrow:
    return "arrow";
  case minusminus:
    return "minusminus";
  case tilde:
    return "tilde";
  case exclaim:
    return "exclaim";
  case exclaimequal:
    return "exclaimequal";
  case slash:
    return "slash";
  case percent:
    return "percent";
  case less:
    return "less";
  case lessequal:
    return "lessequal";
  case greater:
    return "greater";
  case greaterequal:
    return "greaterequal";
  case caret:
    return "caret";
  case pipe:
    return "pipe";
  case pipepipe:
    return "pipepipe";
  case question:
    return "question";
  case colon:
    return "colon";
  case semi:
    return "semi";
  case equal:
    return "equal";
  case equalequal:
    return "equalequal";
  case comma:
    return "comma";
  case blank:
    return "blank";
  case eol:
    return "eol";
  case eof:
    return "eof";
  default:
    return "token(" + std::to_string(kind) + ")";
  }
}

TokenKind from_kind_name(const std::string &kind) {
  if (kind == "comment")
    return tok::comment;
  if (kind == "extern")
    return tok::extern_;
  if (kind == "typedef")
    return tok::typedef_;
  if (kind == "if")
    return tok::if_;
  if (kind == "else")
    return tok::else_;
  if (kind == "for")
    return tok::for_;
  if (kind == "continue")
    return tok::continue_;
  if (kind == "break")
    return tok::break_;
  if (kind == "syntax")
    return tok::syntax;
  if (kind == "func")
    return tok::func;
  if (kind == "var")
    return tok::var;
  if (kind == "identifier")
    return tok::identifier;
  if (kind == "char_constant")
    return tok::char_constant;
  if (kind == "numeric_constant")
    return tok::numeric_constant;
  if (kind == "string_literal")
    return tok::string_literal;
  if (kind == "l_square")
    return tok::l_square;
  if (kind == "r_square")
    return tok::r_square;
  if (kind == "l_paren")
    return tok::l_paren;
  if (kind == "r_paren")
    return tok::r_paren;
  if (kind == "l_brace")
    return tok::l_brace;
  if (kind == "r_brace")
    return tok::r_brace;
  if (kind == "period")
    return tok::period;
  if (kind == "amp")
    return tok::amp;
  if (kind == "ampamp")
    return tok::ampamp;
  if (kind == "star")
    return tok::star;
  if (kind == "plus")
    return tok::plus;
  if (kind == "plusplus")
    return tok::plusplus;
  if (kind == "minus")
    return tok::minus;
  if (kind == "arrow")
    return tok::arrow;
  if (kind == "minusminus")
    return tok::minusminus;
  if (kind == "tilde")
    return tok::tilde;
  if (kind == "exclaim")
    return tok::exclaim;
  if (kind == "exclaimequal")
    return tok::exclaimequal;
  if (kind == "slash")
    return tok::slash;
  if (kind == "percent")
    return tok::percent;
  if (kind == "less")
    return tok::less;
  if (kind == "lessequal")
    return tok::lessequal;
  if (kind == "greater")
    return tok::greater;
  if (kind == "greaterequal")
    return tok::greaterequal;
  if (kind == "caret")
    return tok::caret;
  if (kind == "pipe")
    return tok::pipe;
  if (kind == "pipepipe")
    return tok::pipepipe;
  if (kind == "question")
    return tok::question;
  if (kind == "colon")
    return tok::colon;
  if (kind == "semi")
    return tok::semi;
  if (kind == "equal")
    return tok::equal;
  if (kind == "equalequal")
    return tok::equalequal;
  if (kind == "comma")
    return tok::comma;
  if (kind == "blank")
    return tok::blank;
  if (kind == "eol")
    return tok::eol;
  if (kind == "eof")
    return tok::eof;
  return tok::unknown;
}

} // namespace cdhs::tok
