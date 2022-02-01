//
// Created by chudonghao on 2022/1/28.
//

#include "Lexer.h"

#include <iostream>
#include <stdexcept>

#include <boost/spirit/include/lex_lexertl.hpp>

#include "CompileError.h"
#include "Token.h"

namespace lex = boost::spirit::lex;

namespace {
using namespace cdhs;
template <typename Lexer>
struct _lexer : lex::lexer<Lexer> {
  _lexer() {
    this->self.add(R"( |\t)", tok::blank);
    this->self.add(R"(\r\n|\r|\n|\n\r)", tok::eol);

    this->self.add(R"([a-zA-Z_]\w+)", tok::identifier);
    this->self.add(R"(\'(\\.|.)\')", tok::char_constant);
    this->self.add(R"(\d*(\.\d*)?([eE]-?\d+)?)", tok::numeric_constant);
    this->self.add(R"(\"([^"\\]|\\.)*\")", tok::string_literal);

    this->self.add(R"(\/\/[^\r\n]*)", tok::comment);
    // this->self.add(R"(\/\*.*?\*\/)", tok::comment); // 暂不支持行内注释

    this->self.add(R"(\[)", tok::l_square);       //[
    this->self.add(R"(\])", tok::r_square);       //]
    this->self.add(R"(\()", tok::l_paren);        //(
    this->self.add(R"(\))", tok::r_paren);        //)
    this->self.add(R"(\{)", tok::l_brace);        //{
    this->self.add(R"(\})", tok::r_brace);        //}
    this->self.add(R"(\.)", tok::period);         //.
    this->self.add(R"(\&\&)", tok::ampamp);       //&&
    this->self.add(R"(\&)", tok::amp);            //&
    this->self.add(R"(\*)", tok::star);           //*
    this->self.add(R"(\+\+)", tok::plusplus);     //++
    this->self.add(R"(\+)", tok::plus);           //+
    this->self.add(R"(\-\>)", tok::arrow);        //->
    this->self.add(R"(\-\-)", tok::minusminus);   //--
    this->self.add(R"(\-)", tok::minus);          //-
    this->self.add(R"(\~)", tok::tilde);          //~
    this->self.add(R"(\!\=)", tok::exclaimequal); //!=
    this->self.add(R"(\!)", tok::exclaim);        //!
    this->self.add(R"(\/)", tok::slash);          ///
    this->self.add(R"(\%)", tok::percent);        //%
    this->self.add(R"(\<\=)", tok::lessequal);    //<=
    this->self.add(R"(\<)", tok::less);           //<
    this->self.add(R"(\>=)", tok::greaterequal);  //>=
    this->self.add(R"(\>)", tok::greater);        //>
    this->self.add(R"(\^)", tok::caret);          //^
    this->self.add(R"(\|\|)", tok::pipepipe);     //||
    this->self.add(R"(\|)", tok::pipe);           //|
    this->self.add(R"(\?)", tok::question);       //?
    this->self.add(R"(\:)", tok::colon);          //:
    this->self.add(R"(\;)", tok::semi);           //;
    this->self.add(R"(\=\=)", tok::equalequal);   //==
    this->self.add(R"(\=)", tok::equal);          //=
    this->self.add(R"(\,)", tok::comma);          //,

    this->self.add(R"(.)", tok::unknown);
  }

  void reset(std::string *source) {
    m_begin = &(*source)[0];
    m_end = &(*source)[source->size()];
    m_inited = false;
    m_token_row = 0;
    m_token_column = 0;
  }

  void advance() {
    updateRowColumn();
    if (m_inited) {
      ++m_iter;
    } else {
      m_iter = lex::lexer<Lexer>::begin(m_begin, m_end);
      m_inited = true;
    }
  }

  bool tokenValid() const { return m_iter != lex::lexer<Lexer>::end() && token().is_valid(); }
  std::string tokenSource() const {
    if (!tokenValid()) {
      return {};
    }
    return std::string(token().value().begin(), token().value().end());
  }
  tok::TokenKind tokenKind() const {
    if (!tokenValid()) {
      return tok::unknown;
    }
    return (tok::TokenKind)token().id();
  }
  size_t tokenRow() const { return m_token_row; }
  size_t tokenColumn() const { return m_token_column; }
  size_t tokenLength() const {
    if (!tokenValid()) {
      return 0;
    }
    return token().value().size();
  };
  SourceLocation tokenSourceLocation() const {
    SourceLocation sl;
    sl.row = m_token_row;
    sl.column = m_token_column;
    sl.len = tokenLength();
    return sl;
  }

private:
  const typename Lexer::token_type &token() const { return *m_iter; };

  void updateRowColumn() {
    if (!tokenValid()) {
      return;
    }
    if (tokenKind() == tok::eol) {
      ++m_token_row;
      m_token_column = 0;
    } else {
      m_token_column += tokenLength();
    }
  }

  bool m_inited{false};
  const char *m_begin{};
  const char *m_end{};
  typename _lexer::iterator_type m_iter;
  size_t m_token_row{};
  size_t m_token_column{};
};

_lexer<lex::lexertl::lexer<>> &as_lexer(void *p) { return *(_lexer<lex::lexertl::lexer<>> *)p; }

} // namespace

namespace cdhs {

Lexer::Lexer() { m_internal_lexer = new _lexer<lex::lexertl::lexer<>>; }

Lexer::~Lexer() { delete (_lexer<lex::lexertl::lexer<>> *)m_internal_lexer; }

int Lexer::lex(size_t n_token) {
  while (n_token > m_tokens.size()) {
    int res = lexOneToken();
    if (res != 0) {
      return -1;
    }
  }
  return 0;
}

int Lexer::advance(size_t n_token) {
  for (int i = 0; i < n_token; ++i) {
    if (m_tokens.empty()) {
      return -1;
    }
    m_tokens.pop_front();
  }
  return 0;
}

int Lexer::advanceAndLex(size_t advance_n_token, size_t lex_n_token) {
  advance(advance_n_token);
  return lex(lex_n_token);
}

void Lexer::reset(std::string *source) {
  as_lexer(m_internal_lexer).reset(source);
  m_tokens.clear();
}

int Lexer::getTokenCount() const { return m_tokens.size(); }

const Token &Lexer::getToken(size_t index) const {
  if (index > m_tokens.size()) {
    throw std::logic_error("tokens[" + std::to_string(index) + "] is invalid");
  }
  return m_tokens[index];
}

int Lexer::lexOneToken() {
  auto &lexer = as_lexer(m_internal_lexer);

try_again:

  lexer.advance();

  if (!lexer.tokenValid()) {
    // 到达末尾了
    if (!m_tokens.empty()) {
      if (m_tokens.back().kind == tok::eof) {
        return -1;
      }
    }
    m_tokens.emplace_back(lexer.tokenSourceLocation(), tok::eof);
    return 0;
  }

  switch (lexer.tokenKind()) {
  case tok::comment:
  case tok::blank:
  case tok::eol:
    goto try_again;
  case tok::identifier:
  case tok::char_constant:
  case tok::numeric_constant:
  case tok::string_literal:
  case tok::l_square:
  case tok::r_square:
  case tok::l_paren:
  case tok::r_paren:
  case tok::l_brace:
  case tok::r_brace:
  case tok::period:
  case tok::amp:
  case tok::ampamp:
  case tok::star:
  case tok::plus:
  case tok::plusplus:
  case tok::minus:
  case tok::arrow:
  case tok::minusminus:
  case tok::tilde:
  case tok::exclaim:
  case tok::exclaimequal:
  case tok::slash:
  case tok::percent:
  case tok::less:
  case tok::lessequal:
  case tok::greater:
  case tok::greaterequal:
  case tok::caret:
  case tok::pipe:
  case tok::pipepipe:
  case tok::question:
  case tok::colon:
  case tok::semi:
  case tok::equal:
  case tok::equalequal:
  case tok::comma:
    m_tokens.emplace_back(lexer.tokenSourceLocation(), lexer.tokenSource(), lexer.tokenKind());
    break;
  case tok::eof:
    m_tokens.emplace_back(lexer.tokenSourceLocation(), tok::eof);
    break;
  case tok::tok_max:
  case tok::unknown:
    throw CompileError(lexer.tokenSourceLocation());
  }

  // auto is_space_or_return = [](int ch) { return std::isspace(ch) || ch == '\n' || ch == '\r'; };
  // auto is_eof_or_return = [](int ch) { return ch == EOF || ch == '\n' || ch == '\r'; };
  //
  //// 首次读取
  // if (m_last_char == 0) {
  //   m_last_char = this->getchar();
  // }
  //
  //// 跳过空白符号
  // while (is_space_or_return(m_last_char)) {
  //   m_last_char = this->getchar();
  // }
  //
  //// 跳过注释
  // while (m_last_char == '#') {
  //   while (!is_eof_or_return(m_last_char)) {
  //     m_last_char = this->getchar();
  //   }
  //   // 跳过所有空白符号
  //   while (is_space_or_return(m_last_char)) {
  //     m_last_char = this->getchar();
  //   }
  // }
  //
  //// 读取标识符
  // if (std::isalpha(m_last_char)) {
  //   std::string id;
  //   while (std::isalnum(m_last_char)) {
  //     id.push_back(m_last_char);
  //     m_last_char = this->getchar();
  //   }
  //   m_tokens.push_back();
  // }
  //
  // return 0;

  return 0;
}
//
// int Lexer::getchar() {
//  if (!m_source) {
//    return EOF;
//  }
//  ++m_pos;
//  if (m_pos >= m_source->size()) {
//    return EOF;
//  }
//  return (*m_source)[m_pos];
//}

} // namespace cdhs
