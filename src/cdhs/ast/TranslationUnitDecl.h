//
// Created by chudonghao on 2022/2/2.
//

#ifndef INC_2022_2_2_2DEB5AFDA45C46C4934F2291059241CE_H
#define INC_2022_2_2_2DEB5AFDA45C46C4934F2291059241CE_H

#include <memory>
#include <vector>

#include "Stmt.h"

namespace cdhs::ast {

/// TranslationUnitDecl ::= (Stmt ";") *
struct TranslationUnitDecl {
  std::vector<std::unique_ptr<Stmt>> Stmts;
};

} // namespace cdhs::ast

#endif // INC_2022_2_2_2DEB5AFDA45C46C4934F2291059241CE_H
