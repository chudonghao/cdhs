//
// Created by chudonghao on 2022/1/28.
//

#include "SourceLocation.h"

#include <cstdio>

namespace cdhs {

void SourceLocation::advance(int c) {
  if (c == '\n') {
    ++row;
    column = 0;
    return;
  }

  if (c == '\r' || c == EOF) {
    // nothing
    return;
  }

  ++column;
}

} // namespace cdhs