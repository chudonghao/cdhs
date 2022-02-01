//
// Created by chudonghao on 2022/2/1.
//

#ifndef INC_2022_2_1_86AE91761D984709AC156A1A6A633B2D_H
#define INC_2022_2_1_86AE91761D984709AC156A1A6A633B2D_H

#include <memory>

namespace cdhs::ast {

struct Primary;

struct S {
  enum Select {
    SELECT_e,        //
    SELECT_Primary_S //
  } select{};

  std::shared_ptr<Primary> Primary;
  std::shared_ptr<S> S;
};

} // namespace cdhs::ast

#endif // INC_2022_2_1_86AE91761D984709AC156A1A6A633B2D_H
