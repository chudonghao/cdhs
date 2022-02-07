//
// Created by chudonghao on 2022/2/2.
//

#ifndef INC_2022_2_2_A6163E5847514C9187C45C33AD4566B6_H
#define INC_2022_2_2_A6163E5847514C9187C45C33AD4566B6_H

#include <memory>

#include "Stmt.h"

namespace cdhs::ast {

/// ForStmt ::= "for" "(" ValueStmt | e ";" ValueStmt | e ";" ValueStmt | e ")" CompoundStmt | e
struct ForStmt : Stmt {};

} // namespace cdhs::ast

#endif // INC_2022_2_2_A6163E5847514C9187C45C33AD4566B6_H
