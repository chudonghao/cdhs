//
// Created by chudonghao on 2022/1/30.
//

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <cdhs/ExprParser.h>
#include <cdhs/Grammar.h>

using namespace cdhs;

namespace {

void print_Productions(ProductionVecConstRef ps) {
  std::cout << "Productions:" << std::endl;
  for (const auto &p : ps) {
    if (p.lookahead.empty()) {
      std::cout << p << std::endl;
    } else {
      std::cout << p << " , " << p.lookahead << std::endl;
    }
  }
  std::cout << std::endl;
}

void print_FIRSTS(const Grammar &g, const GrammarAnalyzer::FIRSTS_Type &firsts) {
  std::set<std::string> viewed;
  std::cout << "FIRST(X): " << std::endl;
  for (const auto &p : g.P) {
    auto &l = p.l;
    if (viewed.count(l)) {
      continue;
    } else {
      viewed.insert(l);
    }

    if (!firsts.count(l)) {
      continue;
    }
    auto &fs = firsts.at(l);
    std::cout << "FIRST(" + l + ")= ";
    for (const auto &f : fs) {
      std::cout << f << ' ';
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  viewed.clear();
}

void print_FOLLOWS(const Grammar &g, const GrammarAnalyzer::FOLLOWS_Type &firsts) {
  std::set<std::string> viewed;
  std::cout << "FOLLOWS(X): " << std::endl;
  for (const auto &p : g.P) {
    auto &l = p.l;
    if (viewed.count(l)) {
      continue;
    } else {
      viewed.insert(l);
    }

    if (!firsts.count(l)) {
      continue;
    }
    auto &fs = firsts.at(l);
    std::cout << "FOLLOWS(" + l + ")= ";
    for (const auto &f : fs) {
      std::cout << f << ' ';
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  viewed.clear();
}

void print_SELECT(const Production &p, const GrammarAnalyzer::SELECTS_Type::mapped_type &ss) {
  auto &l = p.l;
  auto &rs = p.r;
  std::cout << "SELECT(" << p << ")= ";
  for (const auto &f : ss) {
    std::cout << f << ' ';
  }
  std::cout << std::endl;
}

void print_SELECTS(const Grammar &g, const GrammarAnalyzer::SELECTS_Type &selects) {
  std::cout << "SELECT(X): " << std::endl;
  for (const auto &p : g.P) {
    if (selects.count(p)) {
      print_SELECT(p, selects.at(p));
    }
  }
  std::cout << std::endl;
}

void print_LL1_CHECK(const Grammar &g, const GrammarAnalyzer::SELECTS_Type &selects) {
  std::set<std::string> viewed;
  std::cout << "LL1 CHECK: " << std::endl;
  for (auto iter = g.P.begin(); iter != g.P.end(); ++iter) {
    auto &p = *iter;
    auto &l = p.l;
    if (!selects.count(p)) {
      continue;
    }
    auto &ss = selects.at(p);
    if (viewed.count(l)) {
      continue;
    } else {
      viewed.insert(l);
    }

    for (auto iter2 = std::next(iter); iter2 != g.P.end(); ++iter2) {
      auto &p2 = *iter2;
      auto &l2 = p2.l;
      if (l2 != l || !selects.count(p2)) {
        continue;
      }
      auto &ss2 = selects.at(p2);

      std::vector<std::string> iss;
      std::set_intersection(ss.begin(), ss.end(), ss2.begin(), ss2.end(), std::back_inserter(iss));
      if (!iss.empty()) {
        std::cout << "LL1 CHECK FAILED: " << std::endl;
        print_SELECT(p, selects.at(p));
        print_SELECT(p2, selects.at(p2));
      }
    }
  }
}

void print_LR_C(const Grammar &g, const std::vector<ProductionVec> &C) {
  for (int i = 0; i < C.size(); ++i) {
    std::cout << "state " << i << ":" << std::endl;
    print_Productions(C[i]);
  }
}

void print_LR_ParseTable(const Grammar &g, const std::vector<ProductionVec> &C, const GrammarAnalyzer::LR0ParseTable &table) {
  auto state_count = C.size();
  auto &V_T = g.V_T;
  auto &V_N = g.V_N;

  std::cout << "Parse Table: " << std::endl;
  std::cout << "state\\symbol\t";
  for (const auto &t : V_T) {
    std::cout << t << "\t";
  }
  for (const auto &n : V_N) {
    std::cout << n << "\t";
  }
  std::cout << std::endl;
  for (int i = 0; i < state_count; ++i) {
    std::cout << i << "\t";
    for (const auto &t : V_T) {
      std::string does;
      if (table.count({i, t})) {
        std::vector<std::string> table_cell;
        auto &tc = table.at({i, t});
        std::transform(tc.begin(), tc.end(), std::back_inserter(table_cell), &LR0ParseTable::to_string);
        does = boost::join(table_cell, "/");
      }
      std::cout << does << "\t";
    }
    for (const auto &n : V_N) {
      std::string does;
      if (table.count({i, n})) {
        std::vector<std::string> table_cell;
        auto &tc = table.at({i, n});
        std::transform(tc.begin(), tc.end(), std::back_inserter(table_cell), &LR0ParseTable::to_string);
        does = boost::join(table_cell, "/");
      }
      std::cout << does << "\t";
    }
    std::cout << std::endl;
  }
}

} // namespace

int main() {
  auto is_terminal = [](const std::string &x) {
    std::set<std::string> terminals = {};
    return !std::isalpha(x[0]) || !std::isupper(x[0]) || terminals.find(x) != terminals.end();
  };

  // const Grammar &g = Grammar::create(
  //     {
  //         {"E", {"E", "+", "T"}},
  //         {"E", {"T"}},
  //         {"T", {"T", "*", "F"}},
  //         {"T", {"F"}},
  //         {"F", {"(", "E", ")"}},
  //         {"F", {"id"}},
  //     },
  //     "E", "e", is_terminal);

  // const Grammar &g = Grammar::create(
  //    {
  //        {"T", {"a", "B", "d"}},
  //        {"T", {}},
  //        {"B", {"T", "b"}},
  //        {"B", {}},
  //    },
  //    "T", "e", is_terminal);

  // const Grammar &g = Grammar::create(
  //    {
  //        {"S", {"L", "=", "R"}},
  //        {"S", {"R"}},
  //        {"L", {"*", "R"}},
  //        {"L", {"id"}},
  //        {"R", {"L"}},
  //    },
  //    "S", "e", is_terminal);

  // const Grammar &g = Grammar::create(
  //     {
  //         {"StringLiterals", {"string_literal"}},
  //         {"StringLiterals", {"StringLiterals", "string_literal"}},
  //         {"FuncArgs", {"expr"}},
  //         {"FuncArgs", {"FuncArgs", ",", "expr"}},
  //         {"Term", {"identifier"}},
  //         {"Term", {"char_constant"}},
  //         {"Term", {"numeric_constant"}},
  //         {"Term", {"StringLiterals"}},
  //         {"Term", {"identifier", "(", "FuncArgs", ")"}},
  //         {"Term", {"identifier", "(", ")"}},
  //     },
  //     "Term", "e", is_terminal);

  cdhs::ExprParser ep;
  const Grammar &g = *ep.getGrammar();

  std::cout << "V_T: ";
  for (const auto &t : g.V_T) {
    std::cout << t << ' ';
  }
  std::cout << std::endl;
  std::cout << "V_N: ";
  for (const auto &n : g.V_N) {
    std::cout << n << ' ';
  }
  std::cout << std::endl;
  std::cout << "P: " << std::endl;
  for (int i = 0; i < g.P.size(); ++i) {
    std::cout << "    " << i << " " << g.P[i] << std::endl;
  }

  std::cout << "S: " << g.S << std::endl;
  std::cout << "FOLLOW(S): $" << std::endl;
  std::cout << "EMPTY_DEF: " << g.EMPTY_DEF << std::endl;
  std::cout << std::endl;

  auto ga = GrammarAnalyzer::analyze(g);
  // auto ga = GrammarAnalyzer::analyze(g, GrammarAnalyzer::GRAMMAR_TYPE_ALL);
  print_FIRSTS(g, ga.firsts);
  print_FOLLOWS(g, ga.follows);
  print_SELECTS(g, ga.selects);
  print_Productions(ga.lr_items);
  print_LL1_CHECK(g, ga.selects);
  print_LR_C(g, ga.lr0_C);
  // print_LR_C(g, ga.lr1_C);
  std::cout << "LR0 Grammar: " << std::boolalpha << ga.isLr0Grammar() << std::endl;
  print_LR_ParseTable(g, ga.lr0_C, ga.lr0_parse_table);
  std::cout << "SLR Grammar: " << ga.isSlrGrammar() << std::endl;
  print_LR_ParseTable(g, ga.lr0_C, ga.slr_parse_table);
  // std::cout << "LR1 Grammar: " << ga.isLr1Grammar() << std::endl;
  // print_LR_ParseTable(g, ga.lr1_C, ga.lr1_parse_table);
  return 0;
}