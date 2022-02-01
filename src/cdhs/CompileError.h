//
// Created by chudonghao on 2022/2/1.
//

#ifndef INC_2022_2_1_A701E564AE624D31B8F3B7AC1192D88C_H
#define INC_2022_2_1_A701E564AE624D31B8F3B7AC1192D88C_H

#include <stdexcept>

#include "SourceLocation.h"

namespace cdhs {

class CompileError : public std::runtime_error {
public:
  explicit CompileError(SourceLocation sl);

  // TODO more info
  // CompileError();
private:
  SourceLocation m_source_location;
};

} // namespace cdhs

#endif // INC_2022_2_1_A701E564AE624D31B8F3B7AC1192D88C_H
