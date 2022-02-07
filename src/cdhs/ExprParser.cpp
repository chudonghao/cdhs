//
// Created by chudonghao on 2022/2/2.
//

#include "ExprParser.h"

#include <atomic>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include "CompileError.h"
#include "Grammar.h"
#include "Lexer.h"
#include "ast/Expr.h"

using namespace cdhs;

namespace {

ProductionVec ps_simple = {
    {"Expr", {"Term14"}},
    {"Term", {"identifier"}},
    {"Term", {"char_constant"}},
    {"Term", {"numeric_constant"}},
    {"Term3", {"Term"}},
    {"Term3", {"Term3", "*", "Term"}},
    {"Term3", {"Term3", "/", "Term"}},
    {"Term4", {"Term3"}},
    {"Term4", {"Term4", "+", "Term3"}},
    {"Term4", {"Term4", "-", "Term3"}},
    {"Term14", {"Term4"}},
    {"Term14", {"Term4", "=", "Term14"}},
};

ProductionVec ps = {
    {"Expr", {"Term15"}},
    {"StringLiterals", {"string_literal"}},
    {"StringLiterals", {"StringLiterals", "string_literal"}},
    {"FuncArgs", {"Term14"}},
    {"FuncArgs", {"FuncArgs", ",", "Term14"}},
    {"Term", {"identifier"}},
    {"Term", {"char_constant"}},
    {"Term", {"numeric_constant"}},
    {"Term", {"StringLiterals"}},
    {"Term", {"identifier", "(", "FuncArgs", ")"}},
    {"Term", {"identifier", "(", ")"}},
    {"Term1", {"Term"}},
    {"Term1", {"Term1", ".", "identifier"}},
    {"Term1", {"Term1", "->", "identifier"}},
    {"Term1", {"(", "Term14", ")"}},
    {"Term1", {"Term1", "[", "Term14", "]"}},
    {"Term2", {"Term1"}},
    {"Term2", {"!", "Term2"}},
    {"Term2", {"-", "Term2"}},
    {"Term2", {"~", "Term2"}},
    {"Term2", {"++", "Term2"}},
    {"Term2", {"--", "Term2"}},
    {"Term2", {"*", "Term2"}},
    {"Term2", {"&", "Term2"}},
    {"Term2", {"Term1", "++"}},
    {"Term2", {"Term1", "--"}},
    {"Term3", {"Term2"}},
    {"Term3", {"Term3", "*", "Term2"}},
    {"Term3", {"Term3", "/", "Term2"}},
    {"Term3", {"Term3", "%", "Term2"}},
    {"Term4", {"Term3"}},
    {"Term4", {"Term4", "+", "Term3"}},
    {"Term4", {"Term4", "-", "Term3"}},
    {"Term5", {"Term4"}},
    {"Term6", {"Term5"}},
    {"Term6", {"Term6", ">", "Term5"}},
    {"Term6", {"Term6", ">=", "Term5"}},
    {"Term6", {"Term6", "<", "Term5"}},
    {"Term6", {"Term6", "<=", "Term5"}},
    {"Term7", {"Term6"}},
    {"Term7", {"Term7", "==", "Term6"}},
    {"Term7", {"Term7", "!=", "Term6"}},
    {"Term8", {"Term7"}},
    {"Term8", {"Term8", "&", "Term7"}},
    {"Term9", {"Term8"}},
    {"Term9", {"Term9", "^", "Term8"}},
    {"Term10", {"Term9"}},
    {"Term10", {"Term10", "|", "Term9"}},
    {"Term11", {"Term10"}},
    {"Term11", {"Term11", "&&", "Term10"}},
    {"Term12", {"Term11"}},
    {"Term12", {"Term12", "||", "Term11"}},
    {"Term13", {"Term12"}},
    {"Term13", {"Term12", "?", "Term13", ":", "Term13"}},
    {"Term14", {"Term13"}},
    {"Term14", {"Term13", "=", "Term14"}},
    {"Term15", {"Term14"}},
    {"Term15", {"Term15", ",", "Term14"}},
};

int create_symbol_index(const std::string &symbol) {
  auto kind = tok::from_kind_name(symbol);
  if (kind > 0 && kind < tok::tok_max) {
    return kind;
  }

  std::map<std::string, int> symbol2Index = {
      {"identifier", tok::identifier},
      {"char_constant", tok::char_constant},
      {"numeric_constant", tok::numeric_constant},
      {"string_literal", tok::string_literal},
      {"[", tok::l_square},
      {"]", tok::r_square},
      {"(", tok::l_paren},
      {")", tok::r_paren},
      {"{", tok::l_brace},
      {"}", tok::r_brace},
      {".", tok::period},
      {"&", tok::amp},
      {"&", tok::ampamp},
      {"*", tok::star},
      {"+", tok::plus},
      {"++", tok::plusplus},
      {"-", tok::minus},
      {"->", tok::arrow},
      {"--", tok::minusminus},
      {"~", tok::tilde},
      {"!", tok::exclaim},
      {"!=", tok::exclaimequal},
      {"/", tok::slash},
      {"%", tok::percent},
      {"<", tok::less},
      {"<=", tok::lessequal},
      {">", tok::greater},
      {">=", tok::greaterequal},
      {"^", tok::caret},
      {"|", tok::pipe},
      {"||", tok::pipepipe},
      {"?", tok::question},
      {":", tok::colon},
      {";", tok::semi},
      {"=", tok::equal},
      {"==", tok::equalequal},
      {",", tok::comma},
  };

  if (auto iter = symbol2Index.find(symbol); iter != symbol2Index.end()) {
    return iter->second;
  }

  static std::atomic<int> index = 0;
  return tok::tok_max + index++;
}

} // namespace

namespace cdhs {

ExprParser::ExprParser() {
  m_grammar = std::make_shared<Grammar>();
  // 创建文法
  auto is_terminal = [](const std::string &x) {
    std::set<std::string> terminals = {};
    return !std::isalpha(x[0]) || !std::isupper(x[0]) || terminals.find(x) != terminals.end();
  };
  *m_grammar = Grammar::create(ps, "Expr", "e", is_terminal);

  // 分析文法
  GrammarAnalyzer ga = GrammarAnalyzer::analyze(*m_grammar);
  if (!ga.isSlrGrammar() && !ga.isLr1Grammar()) {
    std::cerr << "NOT SLR Grammar" << std::endl;
    return;
  }

  // 创建符号映射
  // 特殊处理:和;和$
  m_symbol2IndexMap[";"] = create_symbol_index(";");
  m_symbol2IndexMap[")"] = create_symbol_index(")");
  m_symbol2IndexMap["$"] = tok::eof;
  for (const auto &t : m_grammar->V_T) {
    if (m_symbol2IndexMap.count(t)) {
      continue;
    }
    m_symbol2IndexMap[t] = create_symbol_index(t);
  }
  for (const auto &n : m_grammar->V_N) {
    if (m_symbol2IndexMap.count(n)) {
      continue;
    }
    m_symbol2IndexMap[n] = create_symbol_index(n);
  }
  for (const auto &p : m_symbol2IndexMap) {
    m_index2SymbolMap[p.second] = p.first;
  }

  // 创建分析表
  m_parse_table = ga.createSlrParseTable([=](auto &str) { return this->toSymbolIndex(str); });

  // ;和)是Expr的FOLLOW集，在表达式可终止且遇到它们时，将它们视为终止符
  for (int i = 0; i < m_parse_table.getStateSize(); ++i) {
    LR0ParseTable::Action action_semi = m_parse_table.getAction(i, toSymbolIndex(";"));
    LR0ParseTable::Action action_r_paren = m_parse_table.getAction(i, toSymbolIndex(")"));
    LR0ParseTable::Action action_stop = m_parse_table.getAction(i, toSymbolIndex("$"));

    if (action_stop.type != LR0ParseTable::ACTION_TYPE_ERROR) {
      if (action_semi.type == LR0ParseTable::ACTION_TYPE_ERROR) {
        m_parse_table.setAction(i, toSymbolIndex(";"), action_stop);
      }
      if (action_r_paren.type == LR0ParseTable::ACTION_TYPE_ERROR) {
        m_parse_table.setAction(i, toSymbolIndex(")"), action_stop);
      }
    }
  }

  // 转换产生式
  for (const auto &p : m_grammar->P) {
    LR0Production p2;
    p2.l = toSymbolIndex(p.l);
    for (const auto &r : p.r) {
      p2.r.push_back(toSymbolIndex(r));
    }
    m_productions.emplace_back(std::move(p2));
  }

  // 创建将表达式转换为AST的函数
  createParseTermFuncs();
}

std::shared_ptr<const Grammar> ExprParser::getGrammar() const { return m_grammar; } // namespace cdhs

int ExprParser::onReduced(int production_id, int n_symbols) {
  std::cout << "reduce " << m_grammar->P[production_id].l << "(" << production_id << ");" << std::endl;

  std::vector<Term> items(n_symbols);
  for (int i = n_symbols - 1; i >= 0; --i) {
    items[i] = std::move(m_term_stack.back());
    m_term_stack.pop_back();
  }
  m_term_stack.emplace_back(Term(production_id, std::move(items)));
  // printTermStack();
  return 0;
}

int ExprParser::onErrored(int state, int symbol) {
  lexer().lex(1);
  auto &token = lexer().getToken(0);

  std::vector<std::string> terminal_accepted;
  for (const auto &t : m_grammar->V_T) {
    auto action = m_parse_table.getAction(state, toSymbolIndex(t));
    if (action.type != LR0ParseTable::ACTION_TYPE_ERROR) {
      terminal_accepted.push_back(t);
    }
  }

  std::cerr << "state: " << state << " input:" << toSymbolString(symbol) << std::endl;

  std::string what = "expect " + boost::join(terminal_accepted, " ");
  throw CompileError(token.source_location, what);
}

int ExprParser::onMovedIn(int state, int symbol) {
  m_term_stack.emplace_back(lexer().getToken(0));
  lexer().advance(1);
  std::cout << "moved in;" << std::endl;
  printTermStack();
  return 0;
}

int ExprParser::onWentTo(int state, int symbol) {
  std::cout << "goto;" << std::endl;
  printTermStack();
  return 0;
}

int ExprParser::onAccepted() {
  std::cout << "Debug" << m_term_stack.size() << std::endl;
  if (m_term_stack.size() != 1) {
    throw std::logic_error("unknown state");
  }

  // 前序遍历
  struct Frame {
    Term *item{};
    size_t child_index{};
    int indent{};
  };
  std::stack<Frame> stack;
  Frame cur{&m_term_stack.back(), 0, 0};
  while (cur.item || !stack.empty()) {
    while (cur.item) {
      if (cur.item->production >= 0) {
        std::cout << std::string(cur.indent * 4, ' ') << m_grammar->P[cur.item->production].l << std::endl;
      } else {
        std::cout << std::string(cur.indent * 4, ' ') << cur.item->token.source << std::endl;
      }
      stack.push(cur);
      cur = cur.item->children.empty() ? Frame{nullptr, 0, 0} : Frame{&cur.item->children[0], 0, cur.indent + 1};
    }
    auto &top = stack.top();
    if (top.child_index + 1 < top.item->children.size()) {
      cur = Frame{&top.item->children[top.child_index + 1], 0, top.indent + 1};
      ++top.child_index;
    } else {
      cur = Frame{};
      stack.pop();
    }
  }

  return 1 /*结束分析过程*/;
}

std::unique_ptr<ast::Expr> ExprParser::Expr() {
  auto clear = [=]() {
    LR0StateMachine::reset();
    m_term_stack.clear();
  };

  clear();

  try {
    for (;;) {
      lexer().lex(1);
      auto &token = lexer().getToken(0);

      int ret = 1;

      // 正常退出了
      if ((*this)(token.kind)) {
        break;
      }
    }

  } catch (...) {
    clear();
    throw;
  }

  auto expr = parseTerm(m_term_stack.back());

  clear();

  return expr;
}

std::unique_ptr<ast::Expr> ExprParser::parseTerm(const ExprParser::Term &term) {
  if (term.production < 0) {
    throw std::logic_error("invalid term");
  }

  if (term.production > m_grammar->P.size()) {
    throw std::logic_error("invalid production index");
  }

  return m_production2ParseTermFuncMap[term.production](term);
}

void ExprParser::createParseTermFuncs() {
  std::map<Production, int> p2index_;

  for (const auto &p : m_grammar->P) {
    p2index_[p] = p.index;
  }

  auto p2index = [&](const Production &p) {
    auto iter = p2index_.find(p);
    if (iter == p2index_.end()) {
      std::cerr << "grammar not has production " << p << std::endl;
      return (int)p2index_.size();
    }
    return iter->second;
  };

  /// ================
  /// BASE
  m_production2ParseTermFuncMap[p2index({"Term", {"identifier"}})] = [=](const Term &term) {
    if (term.children.size() != 1) {
      throw CompileError("invalid identifier expr");
    }
    auto o = std::make_unique<ast::DeclRefExpr>();
    o->id = term.children[0].token.source;
    return std::move(o);
  };
  m_production2ParseTermFuncMap[p2index({"Term", {"char_constant"}})] = [=](const Term &term) {
    if (term.children.size() != 1) {
      throw CompileError("invalid char_constant expr");
    }
    auto o = std::make_unique<ast::IntegerLiteral>();
    // TODO 处理转义符
    if (term.children[0].token.source.size() == 3) {
      o->unsigned_value = term.children[0].token.source[1];
    }
    return std::move(o);
  };
  m_production2ParseTermFuncMap[p2index({"Term", {"numeric_constant"}})] = [=](const Term &term) {
    if (term.children.size() != 1) {
      throw CompileError("invalid numeric_constant expr");
    }
    auto o = std::make_unique<ast::FloatingLiteral>();
    o->value = std::strtod(term.children[0].token.source.data(), nullptr);
    return std::move(o);
  };
  m_production2ParseTermFuncMap[p2index({"StringLiterals", {"string_literal"}})] = [=](const Term &term) {
    if (term.children.size() != 1) {
      throw CompileError("invalid string literals expr");
    }
    auto o = std::make_unique<ast::StringLiteral>();
    // TODO 处理转义符
    o->str = term.children[0].token.source;
    return std::move(o);
  };
  m_production2ParseTermFuncMap[p2index({"StringLiterals", {"StringLiterals", "string_literal"}})] = [=](const Term &term) {
    if (term.children.size() != 2) {
      throw CompileError("invalid string literals expr");
    }
    auto o_ = parseTerm(term.children[0]);
    auto o = std::unique_ptr<ast::StringLiteral>((ast::StringLiteral *)o_.release());
    // TODO 处理转义符
    o->str += term.children[1].token.source;
    return std::move(o);
  };
  m_production2ParseTermFuncMap[p2index({"FuncArgs", {"Term14"}})] = [=](const Term &term) {
    if (term.children.size() != 1) {
      throw CompileError("invalid func args expr");
    }
    auto o = std::make_unique<ast::CallArgsExpr>();
    o->args.emplace_back(parseTerm(term.children[0]));
    return std::move(o);
  };
  m_production2ParseTermFuncMap[p2index({"FuncArgs", {"FuncArgs", ",", "Term14"}})] = [=](const Term &term) {
    if (term.children.size() != 3) {
      throw CompileError("invalid func args expr");
    }
    auto o_ = parseTerm(term.children[0]);
    auto o = std::unique_ptr<ast::CallArgsExpr>((ast::CallArgsExpr *)o_.release());
    o->args.emplace_back(parseTerm(term.children[2]));
    return std::move(o);
  };
  m_production2ParseTermFuncMap[p2index({"Term1", {"(", "Term14", ")"}})] = [=](const Term &term) {
    if (term.children.size() != 3) {
      throw CompileError("invalid paren expr");
    }
    return parseTerm(term.children[1]);
  };
  m_production2ParseTermFuncMap[p2index({"Term1", {"Term1", "[", "Term14", "]"}})] = [=](const Term &term) {
    if (term.children.size() != 4) {
      throw CompileError("invalid array subscript expr");
    }
    auto o = std::make_unique<ast::ArraySubscriptExpr>();
    o->array = parseTerm(term.children[0]);
    o->subscript = parseTerm(term.children[2]);
    return parseTerm(term.children[1]);
  };

  /// ================
  /// CALL
  m_production2ParseTermFuncMap[p2index({"Term", {"identifier", "(", "FuncArgs", ")"}})] = [=](const Term &term) {
    if (term.children.size() != 4) {
      throw CompileError("invalid call expr");
    }
    auto o = std::make_unique<ast::CallExpr>();
    o->id = term.children[0].token.source;
    auto args = parseTerm(term.children[2]);
    o->args.swap(((ast::CallArgsExpr *)args.get())->args);
    return std::move(o);
  };
  m_production2ParseTermFuncMap[p2index({"Term", {"identifier", "(", ")"}})] = [=](const Term &term) {
    if (term.children.size() != 3) {
      throw CompileError("invalid call expr");
    }
    auto o = std::make_unique<ast::CallExpr>();
    o->id = term.children[0].token.source;
    return std::move(o);
  };

  /// ================
  /// SIMPLE
#define FUNC_SIMPLE(PARENT, CHILDREN)                                                                                                                \
  m_production2ParseTermFuncMap[p2index({PARENT, {CHILDREN}})] = [=](const Term &term) {                                                             \
    if (term.children.empty()) {                                                                                                                     \
      throw CompileError("invalid expr");                                                                                                            \
    }                                                                                                                                                \
    return parseTerm(term.children[0]);                                                                                                              \
  };
  FUNC_SIMPLE("Expr", "Term15");
  FUNC_SIMPLE("Term", "StringLiterals");
  FUNC_SIMPLE("Term15", "Term14");
  FUNC_SIMPLE("Term14", "Term13");
  FUNC_SIMPLE("Term13", "Term12");
  FUNC_SIMPLE("Term12", "Term11");
  FUNC_SIMPLE("Term11", "Term10");
  FUNC_SIMPLE("Term10", "Term9");
  FUNC_SIMPLE("Term9", "Term8");
  FUNC_SIMPLE("Term8", "Term7");
  FUNC_SIMPLE("Term7", "Term6");
  FUNC_SIMPLE("Term6", "Term5");
  FUNC_SIMPLE("Term5", "Term4");
  FUNC_SIMPLE("Term4", "Term3");
  FUNC_SIMPLE("Term3", "Term2");
  FUNC_SIMPLE("Term2", "Term1");
  FUNC_SIMPLE("Term1", "Term");

  /// =============
  /// L_UNARY
#define FUNC_L_UNARY(L, OP, R)                                                                                                                       \
  m_production2ParseTermFuncMap[p2index({L, {OP, R}})] = [=](const Term &term) {                                                                     \
    if (term.children.size() != 2) {                                                                                                                 \
      throw CompileError("invalid unary expr");                                                                                                      \
    }                                                                                                                                                \
    auto o = std::make_unique<ast::UnaryOperator>();                                                                                                 \
    o->side = ast::UnaryOperator::LHS;                                                                                                               \
    o->op = term.children[0].token.kind;                                                                                                             \
    o->expr = parseTerm(term.children[1]);                                                                                                           \
    return std::move(o);                                                                                                                             \
  };
  FUNC_L_UNARY("Term2", "!", "Term2");
  FUNC_L_UNARY("Term2", "-", "Term2");
  FUNC_L_UNARY("Term2", "~", "Term2");
  FUNC_L_UNARY("Term2", "++", "Term2");
  FUNC_L_UNARY("Term2", "--", "Term2");
  FUNC_L_UNARY("Term2", "*", "Term2");
  FUNC_L_UNARY("Term2", "&", "Term2");

  /// =============
  /// R_UNARY
#define FUNC_R_UNARY(L, R, OP)                                                                                                                       \
  m_production2ParseTermFuncMap[p2index({L, {R, OP}})] = [=](const Term &term) {                                                                     \
    if (term.children.size() != 2) {                                                                                                                 \
      throw CompileError("invalid unary expr");                                                                                                      \
    }                                                                                                                                                \
    auto o = std::make_unique<ast::UnaryOperator>();                                                                                                 \
    o->side = ast::UnaryOperator::RHS;                                                                                                               \
    o->op = term.children[1].token.kind;                                                                                                             \
    o->expr = parseTerm(term.children[0]);                                                                                                           \
    return std::move(o);                                                                                                                             \
  };
  FUNC_R_UNARY("Term2", "Term1", "++");
  FUNC_R_UNARY("Term2", "Term1", "--");

  /// =============
  /// BINARY
#define FUNC_BINARY(L, LE, OP, RE)                                                                                                                   \
  m_production2ParseTermFuncMap[p2index({L, {LE, OP, RE}})] = [=](const Term &term) {                                                                \
    if (term.children.size() != 3) {                                                                                                                 \
      throw CompileError("invalid binary expr");                                                                                                     \
    }                                                                                                                                                \
    auto o = std::make_unique<ast::BinaryOperator>();                                                                                                \
    o->l_expr = parseTerm(term.children[0]);                                                                                                         \
    o->op = term.children[1].token.kind;                                                                                                             \
    o->r_expr = parseTerm(term.children[2]);                                                                                                         \
    return std::move(o);                                                                                                                             \
  };
  FUNC_BINARY("Term1", "Term1", ".", "identifier");
  FUNC_BINARY("Term1", "Term1", "->", "identifier");
  FUNC_BINARY("Term3", "Term3", "*", "Term2");
  FUNC_BINARY("Term3", "Term3", "/", "Term2");
  FUNC_BINARY("Term3", "Term3", "%", "Term2");
  FUNC_BINARY("Term4", "Term4", "+", "Term3");
  FUNC_BINARY("Term4", "Term4", "-", "Term3");
  FUNC_BINARY("Term6", "Term6", ">", "Term5");
  FUNC_BINARY("Term6", "Term6", ">=", "Term5");
  FUNC_BINARY("Term6", "Term6", "<", "Term5");
  FUNC_BINARY("Term6", "Term6", "<=", "Term5");
  FUNC_BINARY("Term7", "Term7", "==", "Term6");
  FUNC_BINARY("Term7", "Term7", "!=", "Term6");
  FUNC_BINARY("Term8", "Term8", "&", "Term7");
  FUNC_BINARY("Term9", "Term9", "^", "Term8");
  FUNC_BINARY("Term10", "Term10", "|", "Term9");
  FUNC_BINARY("Term11", "Term11", "&&", "Term10");
  FUNC_BINARY("Term12", "Term12", "||", "Term11");
  FUNC_BINARY("Term14", "Term13", "=", "Term14");
  FUNC_BINARY("Term15", "Term15", ",", "Term14");

  /// =============
  /// CONDITIONAL
  m_production2ParseTermFuncMap[p2index({"Term13", {"Term12", "?", "Term13", ":", "Term13"}})] = [=](const Term &term) {
    if (term.children.size() != 5) {
      throw CompileError("invalid conditional expr");
    }
    auto o = std::make_unique<ast::ConditionalOperator>();
    o->condition = parseTerm(term.children[0]);
    o->true_expr = parseTerm(term.children[2]);
    o->false_expr = parseTerm(term.children[4]);
    return std::move(o);
  };

  for (const auto &p : m_grammar->P) {
    if (m_production2ParseTermFuncMap.find(p.index) == m_production2ParseTermFuncMap.end()) {
      throw std::logic_error("no parse func for production " + to_string(p));
    }
  }
}

void ExprParser::printTermStack() {
  std::cout << "state stack: ";
  for (const auto &term : m_state_stack) {
    std::cout << term << " ";
  }
  std::cout << std::endl;
  std::cout << "term stack: ";
  for (const auto &term : m_term_stack) {
    if (term.production >= 0) {
      std::cout << m_grammar->P[term.production].l << " ";
    } else {
      std::cout << term.token.source << " ";
    }
  }
  std::cout << std::endl;
}

} // namespace cdhs