//
// Created by chudonghao on 2022/2/2.
//

#ifndef INC_2022_2_2_380280EB6B244DDAA82DA10B087A82DD_H
#define INC_2022_2_2_380280EB6B244DDAA82DA10B087A82DD_H

#include "ValueStmt.h"

namespace cdhs::ast {

/// DeclStmt ::= "var" Id Type
struct DeclStmt : ValueStmt {};

} // namespace cdhs::ast

#endif // INC_2022_2_2_380280EB6B244DDAA82DA10B087A82DD_H
