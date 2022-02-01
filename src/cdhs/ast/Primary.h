//
// Created by chudonghao on 2022/2/1.
//

#ifndef INC_2022_2_1_40766458AC174CD5AD352D0F2598FA26_H
#define INC_2022_2_1_40766458AC174CD5AD352D0F2598FA26_H

#include <memory>

namespace cdhs::ast {

struct Sentence;

struct Primary {
  std::shared_ptr<Sentence> Sentence;
};

} // namespace cdhs::ast

#endif // INC_2022_2_1_40766458AC174CD5AD352D0F2598FA26_H
