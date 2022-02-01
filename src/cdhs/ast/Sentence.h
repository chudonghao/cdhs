//
// Created by chudonghao on 2022/2/1.
//

#ifndef INC_2022_2_1_070609A6847D433995869EBAC3E96D9F_H
#define INC_2022_2_1_070609A6847D433995869EBAC3E96D9F_H

#include <memory>

namespace cdhs::ast {

struct Expr;
struct Sentence;

struct Sentences {
  enum Select {
    SELECT_e,                 //
    SELECT_Sentence_Sentences //
  } select{};

  std::shared_ptr<Sentence> Sentence;
  std::shared_ptr<Sentences> Sentences;
};

struct Sentence_1 {
  enum Select {
    SELECT_l_brace_Sentences_r_brace, //
    SELECT_Sentence                   //
  } select{};

  std::shared_ptr<Sentences> Sentences;
  std::shared_ptr<Sentence> Sentence;
};

struct Sentence {
  enum Select {
    SELECT_Expr_, //
    SELECT_if_,   //
    SELECT_for_,  //
  } select{};

  std::shared_ptr<Expr> Expr;
  std::shared_ptr<Sentence_1> Sentence_1;
};

} // namespace cdhs::ast

#endif // INC_2022_2_1_070609A6847D433995869EBAC3E96D9F_H
