//
// Created by chudonghao on 2022/2/2.
//

#ifndef INC_2022_2_2_BC09085D60AE43C38A7BC9933A20D176_H
#define INC_2022_2_2_BC09085D60AE43C38A7BC9933A20D176_H

#include <memory>

#include "Stmt.h"

namespace cdhs::ast {

/// IfStmt ::= "if" "(" ValueStmt ")" CompoundStmt | e
struct IfStmt : Stmt {};

} // namespace cdhs::ast

#endif // INC_2022_2_2_BC09085D60AE43C38A7BC9933A20D176_H
