//
// Created by chudonghao on 2022/2/2.
//

#ifndef INC_2022_2_2_E6C0CF55E4274B189C39DCCD481F9C9C_H
#define INC_2022_2_2_E6C0CF55E4274B189C39DCCD481F9C9C_H

#include "Stmt.h"

namespace cdhs::ast {

/// ValueStmt ::= DeclStmt | Expr
struct ValueStmt : Stmt {};

} // namespace cdhs::ast

#endif // INC_2022_2_2_E6C0CF55E4274B189C39DCCD481F9C9C_H
