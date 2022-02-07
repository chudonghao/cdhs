//
// Created by chudonghao on 2022/2/6.
//

#ifndef INC_2022_2_6_3D72D3BCB829417A9C34A1D06CF8FFA3_H
#define INC_2022_2_6_3D72D3BCB829417A9C34A1D06CF8FFA3_H

#include <functional>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "LR0StateMachine.h"

namespace cdhs {

struct Production {
  std::string l;
  std::vector<std::string> r;
  Production(std::string l, std::vector<std::string> r) : l(std::move(l)), r(std::move(r)) {}

  // 辅助信息
  size_t index{};
  std::string lookahead;
};

static bool operator<(const Production &l, const Production &r) {
  if (l.l < r.l) {
    return true;
  } else if (l.l == r.l) {
    if (l.r < r.r) {
      return true;
    } else if (l.r == r.r) {
      return l.lookahead < r.lookahead;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

static bool operator==(const Production &l, const Production &r) { return l.l == r.l && l.r == r.r && l.lookahead == r.lookahead; }

static bool operator!=(const Production &l, const Production &r) { return !(l == r); }

static std::string to_string(const Production &p) {
  std::string res = p.l + " ->";
  for (const auto &r : p.r) {
    res += " " + r;
  }
  return res;
}

static std::ostream &operator<<(std::ostream &os, const Production &p) {
  os << p.l << " ->";
  for (const auto &r : p.r) {
    os << " " << r;
  }
  return os;
}

typedef std::vector<Production> ProductionVec;
typedef const ProductionVec &ProductionVecConstRef;

class Grammar {
public:
  static Grammar create(ProductionVec P, std::string S, std::string EMPTY_DEF, std::function<bool(const std::string &)> is_terminal);

public:
  std::set<std::string> V_T;
  std::set<std::string> V_N;
  ProductionVec P;
  std::string S;

  std::string EMPTY_DEF;
};

class GrammarAnalyzer {
public:
  typedef std::map<std::string, std::set<std::string>> FIRSTS_Type;
  typedef std::map<std::string, std::set<std::string>> FOLLOWS_Type;
  typedef std::map<Production, std::set<std::string>> SELECTS_Type;
  typedef std::map<std::pair<size_t, std::string>, std::set<cdhs::LR0ParseTable::Action>> LR0ParseTable;

public:
  // bool is_ll0_grammar();

  bool isLr0Grammar() const;

  bool isSlrGrammar() const;

  bool isLr1Grammar() const;

  cdhs::LR0ParseTable createSlrParseTable(std::function<int(const std::string &)> symbol_index_transform) const;

public:
  enum GrammarType {
    // GRAMMAR_TYPE_LL = 1u,
    GRAMMAR_TYPE_LR0 = 1u << 1u,
    GRAMMAR_TYPE_SLR = 1u << 2u,
    GRAMMAR_TYPE_LR1 = 1u << 3u,
    GRAMMAR_TYPE_ALL = 0xffffffffu,
  };

  static GrammarAnalyzer analyze(const Grammar &g, int GRAMMAR_TYPE = GRAMMAR_TYPE_LR0 | GRAMMAR_TYPE_SLR);

  FIRSTS_Type firsts;
  FOLLOWS_Type follows;
  SELECTS_Type selects;
  ProductionVec lr_items;
  std::vector<ProductionVec> lr0_C;
  LR0ParseTable lr0_parse_table;
  LR0ParseTable slr_parse_table;

  std::vector<ProductionVec> lr1_C;
  LR0ParseTable lr1_parse_table;

private:
  GrammarAnalyzer() = default;
};

} // namespace cdhs

#endif // INC_2022_2_6_3D72D3BCB829417A9C34A1D06CF8FFA3_H
