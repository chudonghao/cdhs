//
// Created by chudonghao on 2022/2/2.
//

#ifndef INC_2022_2_2_24A82A3BA82643C2954BF75F481DD955_H
#define INC_2022_2_2_24A82A3BA82643C2954BF75F481DD955_H

namespace cdhs::ast {

/// Stmt ::= IfStmt | ForStmt | ValueStmt
struct Stmt {
  virtual ~Stmt() = default;
};

} // namespace cdhs::ast

#endif // INC_2022_2_2_24A82A3BA82643C2954BF75F481DD955_H
