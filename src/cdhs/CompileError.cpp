//
// Created by chudonghao on 2022/2/1.
//

#include "CompileError.h"

namespace {

std::string formatCompileError(cdhs::SourceLocation sl) { return "[" + to_string(sl) + "] compile error"; }

} // namespace

namespace cdhs {

CompileError::CompileError(SourceLocation sl) : std::runtime_error(formatCompileError(sl)) {}

} // namespace cdhs
