//
// Created by chudonghao on 2022/1/28.
//

#include "TokenKinds.h"

namespace cdhs::tok {

std::string name(cdhs::tok::TokenKind kind) {
  switch (kind) {
  case unknown:
    return "unknown";
  case comment:
    return "comment";
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

} // namespace cdhs::tok
