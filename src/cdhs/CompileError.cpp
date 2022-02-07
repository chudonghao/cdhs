//
// Created by chudonghao on 2022/2/1.
//

#include "CompileError.h"

namespace {

std::string formatCompileError(cdhs::SourceLocation sl, const std::string &what) { return "[" + to_string(sl) + "] " + what; }

std::string formatCompileError(cdhs::SourceLocation sl) { return formatCompileError(sl, "compile error"); }

} // namespace

namespace cdhs {

CompileError::CompileError(SourceLocation sl) : std::runtime_error(formatCompileError(sl)) {}

CompileError::CompileError(SourceLocation sl, const std::string &what) : std::runtime_error(formatCompileError(sl, what)) {}

CompileError::CompileError(const std::string &what) : std::runtime_error(what) {}

} // namespace cdhs
