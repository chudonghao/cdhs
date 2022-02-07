//
// Created by chudonghao on 2022/2/2.
//

#ifndef INC_2022_2_2_027102ABD9B04393B9639FD5EAB56428_H
#define INC_2022_2_2_027102ABD9B04393B9639FD5EAB56428_H

#include <functional>
#include <memory>
#include <utility>

#include "LR0StateMachine.h"
#include "Token.h"

namespace cdhs {

namespace ast {
struct Expr;
}

class Lexer;
class Grammar;

/// 用来专门进行表达式的语法分析工作
class ExprParser : private LR0StateMachine {
public:
  ExprParser();

  std::shared_ptr<const Grammar> getGrammar() const;

private:
  struct Term {
    int production{-1};
    Token token;
    std::vector<Term> children;
    Term() = default;
    Term(Term &&r) noexcept : production(r.production), token(std::move(r.token)), children(std::move(r.children)) {}
    explicit Term(Token token) : token(std::move(token)) {}
    Term(int production, std::vector<Term> children) : production(production), children(std::move(children)) {}
    Term &operator=(Term &&r) noexcept {
      this->~Term();
      new (this) Term(std::move(r));
      return *this;
    }
  };

private:
  int onReduced(int production_id, int n_symbols) override;
  int onErrored(int state, int symbol) override;
  int onMovedIn(int state, int symbol) override;
  int onWentTo(int state, int symbol) override;
  int onAccepted() override;

  std::unique_ptr<ast::Expr> Expr();

  std::unique_ptr<ast::Expr> parseTerm(const Term &term);

  void printTermStack();

private:
  void createParseTermFuncs();

  friend class Parser;
  Lexer &lexer() { return *m_lexer; }
  Lexer *m_lexer{};
  std::shared_ptr<Grammar> m_grammar;

  typedef std::function<std::unique_ptr<ast::Expr>(const Term &term)> ParseTermFunc;
  std::map<int, ParseTermFunc> m_production2ParseTermFuncMap;

  std::vector<Term> m_term_stack;
};

} // namespace cdhs

#endif // INC_2022_2_2_027102ABD9B04393B9639FD5EAB56428_H
