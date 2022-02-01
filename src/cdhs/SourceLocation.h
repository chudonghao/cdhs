//
// Created by chudonghao on 2022/1/28.
//

#ifndef CDHS_SOURCELOCATION_H
#define CDHS_SOURCELOCATION_H

#include <cstddef>
#include <ostream>

namespace cdhs {

struct SourceLocation {
  size_t row{};
  size_t column{};
  size_t len{}; ///< for token

public:
  void advance(int c);
};

static std::ostream &operator<<(std::ostream &os, const SourceLocation &sl) {
  os << sl.row + 1 << ":" << (sl.len > 0 ? sl.column + 1 : sl.column) << "-" << sl.column + sl.len;
  return os;
}

static std::string to_string(const SourceLocation &sl) {
  return std::to_string(sl.row + 1) + ":" + std::to_string(sl.len > 0 ? sl.column + 1 : sl.column) + "-" + std::to_string(sl.column + sl.len);
}

} // namespace cdhs

#endif // CDHS_SOURCELOCATION_H
