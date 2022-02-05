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

const char *LR_PLACE_HOLDER = "•";

struct Production {
  std::string l;
  std::vector<std::string> r;
  Production(std::string l, std::vector<std::string> r) : l(std::move(l)), r(std::move(r)) {}

  // lr 文法需要
  size_t index{};
  std::string lookahead;
};

bool operator<(const Production &l, const Production &r) {
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

bool operator==(const Production &l, const Production &r) { return l.l == r.l && l.r == r.r && l.lookahead == r.lookahead; }

bool operator!=(const Production &l, const Production &r) { return !(l == r); }

std::ostream &operator<<(std::ostream &os, const Production &p) {
  os << p.l << " ->";
  for (const auto &r : p.r) {
    os << " " << r;
  }
  return os;
}

typedef std::map<std::string, std::set<std::string>> FIRSTS_Type;
typedef std::map<std::string, std::set<std::string>> FOLLOWS_Type;
typedef std::map<Production, std::set<std::string>> SELECTS_Type;
typedef std::vector<Production> ProductionVec;
typedef const ProductionVec &ProductionVecConstRef;
typedef std::map<std::pair<size_t, std::string>, std::set<std::string>> LR0ParseTable;

struct Grammar {
  std::set<std::string> V_T;
  std::set<std::string> V_N;
  ProductionVec P;
  std::string S;

  std::string EMPTY_DEF;
};

struct GrammarAnalyze {
  FIRSTS_Type firsts;
  FOLLOWS_Type follows;

  // ll1
  SELECTS_Type selects;

  // lr文法
  ProductionVec lr_items;
  std::vector<ProductionVec> lr0_C;
  std::vector<ProductionVec> lr1_C;
  LR0ParseTable lr0_parse_table;
  LR0ParseTable slr_parse_table;
  LR0ParseTable lr1_parse_table;
};

Grammar create(ProductionVec P, std::string S, std::set<std::string> S_follows, std::string EMPTY_DEF,
               std::function<bool(const std::string &)> is_terminal) {
  Grammar g;
  g.P = std::move(P);
  g.S = std::move(S);
  g.EMPTY_DEF = std::move(EMPTY_DEF);

  auto all_V_T = [&](const Grammar &g) {
    std::set<std::string> res;
    for (const auto &p : g.P) {
      for (const auto &r : p.r) {
        if (is_terminal(r)) {
          res.insert(r);
        }
      }
    }
    return std::move(res);
  };

  auto all_V_N = [&](const Grammar &g) {
    std::set<std::string> res;
    for (const auto &p : g.P) {
      res.insert(p.l);
      for (const auto &r : p.r) {
        if (!is_terminal(r)) {
          res.insert(r);
        }
      }
    }
    return std::move(res);
  };

  g.V_N = all_V_N(g);
  g.V_T = all_V_T(g);
  // $视为特殊的终结符
  g.V_T.insert("$");

  for (int i = 0; i < g.P.size(); ++i) {
    g.P[i].index = i;
  }

  return std::move(g);
}

std::set<std::string> all_grammar_symbol(const Grammar &g) {
  std::set<std::string> res;
  res.insert(g.V_T.begin(), g.V_T.end());
  res.insert(g.V_N.begin(), g.V_N.end());
  return std::move(res);
}

std::set<std::string> FIRST(const Grammar &g, const FIRSTS_Type &firsts, const std::vector<std::string> &ss) {
  std::set<std::string> fs;

  // std::cout << "!!!!!!!!!!! ";
  // for (const auto &item : ss) {
  //   std::cout << item << " ";
  // }
  //
  // std::cout << ">>>> ";

  size_t empty_count = 0;
  for (const auto &s : ss) {
    // 如果元素是终结符，则将其加入到first集中，跳出循环
    // 如果是非终结符，则该非终结符的所有first集的所有内容加入first集中
    if (g.V_T.count(s)) {
      fs.insert(s);
      break;
    } else {
      if (firsts.count(s)) {
        auto &fs2 = firsts.at(s);
        fs.insert(fs2.begin(), fs2.end());
        if (fs2.count(g.EMPTY_DEF)) {
          ++empty_count;
          continue;
        }
      }
      // 未获取到非终结符的FIRST集，或集合不包含空，跳出循环
      break;
    }
  }
  if (empty_count == ss.size()) {
    // 所有符号都能推导出空，则将空加入到FIRST集中
    fs.insert(g.EMPTY_DEF);
  }

  // for (const auto &item : fs) {
  //   std::cout << item << " ";
  // }
  // std::cout << std::endl;

  return std::move(fs);
}

FIRSTS_Type FIRSTS(const Grammar &g) {
  FIRSTS_Type firsts;
  /// 计算所有FIRST集
  size_t count = 0;
  size_t last_count = -1;
  for (; count != last_count;) {
    // 重试
    last_count = count;
    count = 0;
    // 更新所有first集合
    for (auto &item : g.P) {
      auto &l = item.l;
      auto &rs = item.r;
      auto &fs = firsts[l];
      auto nfs = FIRST(g, firsts, rs);
      fs.insert(nfs.begin(), nfs.end());
      count += fs.size();
    }
  }
  return std::move(firsts);
}

FOLLOWS_Type FOLLOWS(const Grammar &g, const FOLLOWS_Type &firsts) {
  FOLLOWS_Type follows;
  /// 计算所有FOLLOW集
  size_t count = 0;
  size_t last_count = -1;

  // 手动添加S的FOLLOW集
  follows[g.S] = {"$"};

  for (; count != last_count;) {

    // 重试
    last_count = count;
    count = 0;

    // 更新所有follow集合
    for (auto &item : g.P) {
      auto &l = item.l;
      auto &rs = item.r;

      // 对每个产生式的右部的每个非终结符，计算其FOLLOW集
      for (int i = 0; i < rs.size(); ++i) {
        auto &r = rs[i];
        // 跳过终结符
        if (g.V_T.count(r)) {
          continue;
        }
        auto &fs = follows[r];
        // 对于非终结符，将其后面的终结符或非终结符的FIRST集加入到FOLLOW集中，
        // 如果这个非终结符FIRST集包含e（空），则继续看下一个元素，
        // 如果到后面没有更多元素了，则将左部的FOLLOW集加入到FOLLOW集
        bool encounter_empty = true;
        for (int j = i + 1; j <= rs.size() && encounter_empty; ++j) {
          encounter_empty = false;

          if (j == rs.size()) {
            auto &fs2 = follows[l];
            fs.insert(fs2.begin(), fs2.end());
            break;
          }
          auto &f = rs[j];
          if (g.V_T.count(f)) {
            fs.insert(f);
          } else {
            if (firsts.count(f)) {
              auto &fs2 = firsts.at(f);
              std::for_each(fs2.begin(), fs2.end(), [&](auto &v) {
                // 如果FIRST集有空，则移到下一个元素并继续当前过程
                // 如果不是空，则加入到FOLLOW集中
                if (v == g.EMPTY_DEF) {
                  encounter_empty = true;
                } else {
                  fs.insert(v);
                }
              });
            }
          }
        }
      }
    }

    std::for_each(follows.begin(), follows.end(), [&](auto &p) { count += p.second.size(); });
  }
  return std::move(follows);
}

SELECTS_Type SELECTS(const Grammar &g, const FIRSTS_Type &firsts, const FOLLOWS_Type &follows) {
  SELECTS_Type selects;
  /// 计算所有SELECT集
  for (auto &p : g.P) {
    auto &ss = selects[p];
    // 计算右部的FIRST集
    std::set<std::string> fs;
    auto &rs = p.r;
    for (const auto &r : rs) {
      // 如果遇到终结符，则停止计算
      if (g.V_T.count(r)) {
        fs.insert(r);
        break;
      }
      if (firsts.count(r)) {
        auto &fs2 = firsts.at(r);
        fs.insert(fs2.begin(), fs2.end());
        // 如果FIRST集合不包含空，则停止计算
        if (fs2.count(g.EMPTY_DEF) == 0) {
          break;
        }
      }
    }

    // 计算SELECT集
    if (fs.count(g.EMPTY_DEF) == 0) {
      ss.insert(fs.begin(), fs.end());
    } else {
      if (follows.count(p.l)) {
        auto &fs2 = follows.at(p.l);
        ss.insert(fs.begin(), fs.end());
        ss.insert(fs2.begin(), fs2.end());
        ss.erase(g.EMPTY_DEF);
      }
    }
  }
  return std::move(selects);
}

namespace lr {

bool is_move_in_state(const Production &item) { return !item.r.empty() && item.r[0] == LR_PLACE_HOLDER; }

bool is_next_symbol_non_terminal(const Grammar &g, const Production &item) {
  for (int i = 0; i < item.r.size() - 1; ++i) {
    if (item.r[i] == LR_PLACE_HOLDER && !g.V_T.count(item.r[i + 1])) {
      return true;
    }
  }
  return false;
}

bool is_next_symbol_X(const Production &item, const std::string &X) {
  if (item.r.size() < 2) {
    return false;
  }
  for (int i = 0; i < item.r.size() - 1; ++i) {
    if (item.r[i] == LR_PLACE_HOLDER) {
      return item.r[i + 1] == X;
    }
  }
  return false;
}

Production next_state(Production p) {
  for (int i = 0; i < p.r.size() - 1; ++i) {
    if (p.r[i] == LR_PLACE_HOLDER) {
      std::swap(p.r[i], p.r[i + 1]);
      break;
    }
  }
  return std::move(p);
}

std::string next_symbol(const Production &item) {
  for (int i = 0; i < item.r.size() - 1; ++i) {
    if (item.r[i] == LR_PLACE_HOLDER) {
      return item.r[i + 1];
    }
  }
  return {};
}

std::vector<std::string> next_symbols(const Production &item) {
  auto iter = std::find(item.r.begin(), item.r.end(), LR_PLACE_HOLDER);
  std::vector<std::string> res;
  if (iter != item.r.end()) {
    res.assign(std::next(iter), item.r.end());
  }
  return std::move(res);
}

std::vector<std::string> next_symbols(const Production &item, const std::string &lookahead) {
  std::vector<std::string> res = next_symbols(item);
  if (!lookahead.empty()) {
    res.push_back(lookahead);
  }
  return std::move(res);
}

Production remove_lr_place_holder(Production p) {
  auto iter = std::find(p.r.begin(), p.r.end(), LR_PLACE_HOLDER);
  if (iter != p.r.end()) {
    p.r.erase(iter);
  }
  return std::move(p);
}

ProductionVec LR_ITEMS(const Grammar &g) {
  ProductionVec items;
  for (const auto &p : g.P) {
    Production p_new = p;
    p_new.r.insert(p_new.r.begin(), LR_PLACE_HOLDER);
    items.push_back(p_new);

    for (int i = 0; i < p_new.r.size() - 1; ++i) {
      std::swap(p_new.r[i], p_new.r[i + 1]);
      items.push_back(p_new);
    }
  }
  return std::move(items);
}

ProductionVec CLOSURE(const Grammar &g, ProductionVecConstRef lr_items, ProductionVec I) {
  size_t last_count = -1;
  // 重复直到没有新的状态
  for (; I.size() != last_count;) {
    last_count = I.size();
    // 遍历每个状态
    for (int i = 0; i < I.size(); ++i) {
      // 遍历每个下一个符号是非终结符的项目
      if (is_next_symbol_non_terminal(g, I[i])) {
        auto nnt = next_symbol(I[i]);
        // 遍历每个等价项目
        for (const auto &p2 : lr_items) {
          auto &p = I[i];
          if (nnt == p2.l && is_move_in_state(p2)) {
            if (std::find(I.begin(), I.end(), p2) == I.end()) {
              I.push_back(p2);
            }
          }
        }
      }
    }
  }

  return std::move(I);
}

ProductionVec GOTO(const Grammar &g, ProductionVecConstRef lr_items, ProductionVecConstRef I, const std::string &X) {
  ProductionVec J;
  for (const auto &item : I) {
    if (is_next_symbol_X(item, X)) {
      J.push_back(next_state(item));
    }
  }
  return CLOSURE(g, lr_items, J);
}

typedef std::function<ProductionVec(const Grammar &g, ProductionVecConstRef lr_items, ProductionVecConstRef I, const std::string &X)> GOTO_FUNC_Type;

/// 求规范LR项集簇
std::vector<ProductionVec> CANONICAL_COLLECTION(const Grammar &g, ProductionVecConstRef lr_items, std::vector<ProductionVec> C_I0,
                                                const GOTO_FUNC_Type &goto_func) {
  auto &C = C_I0;

  auto ags = all_grammar_symbol(g);
  size_t last_count = 0;

  for (; last_count != C.size();) {
    last_count = C.size();
    for (auto i = 0; i < C.size(); ++i) {
      for (const auto &X : ags) {
        auto &I = C[i];
        auto goto_ = goto_func(g, lr_items, I, X);
        if (!goto_.empty() && std::find(C.begin(), C.end(), goto_) == C.end()) {
          C.push_back(goto_);
        }
      }
    }
  }

  return std::move(C);
}

/// 求规范LR0项集簇
std::vector<ProductionVec> CANONICAL_LR0_COLLECTION(const Grammar &g, ProductionVec lr_items) {
  Production lr_item_S_move_in = {"S'", {LR_PLACE_HOLDER, g.S}};
  Production lr_item_S_reduce = {"S'", {g.S, LR_PLACE_HOLDER}};
  lr_items.push_back(lr_item_S_move_in);
  lr_items.push_back(lr_item_S_reduce);
  std::vector<ProductionVec> C = {CLOSURE(g, lr_items, {lr_item_S_move_in})};
  return std::move(CANONICAL_COLLECTION(g, lr_items, C, &GOTO));
}

LR0ParseTable LR_PARSE_TABLE(const Grammar &g, ProductionVecConstRef lr_items, const std::vector<ProductionVec> &C,
                             const std::function<void(LR0ParseTable &table, size_t state, const Production &item)> &reduce) {
  LR0ParseTable table;
  for (int i = 0; i < C.size(); ++i) {
    auto &I = C[i];
    for (const auto &item : I) {
      auto ns = next_symbol(item);
      if (ns.empty()) {
        if (item.l == "S'") {
          table[{i, "$"}].insert("acc");
        } else {
          reduce(table, i, item);
        }
      } else {
        auto I_j = GOTO(g, lr_items, I, ns);
        auto iter = std::find(C.begin(), C.end(), I_j);
        auto j = std::distance(C.begin(), iter);
        if (iter != C.end()) {
          if (g.V_T.count(ns)) {
            table[{i, ns}].insert("s" + std::to_string(j));
          } else if (g.V_N.count(ns)) {
            table[{i, ns}].insert(std::to_string(j));
          } else {
            throw std::logic_error("??");
          }
        }
      }
    }
  }
  return std::move(table);
}

LR0ParseTable LR0_PARSE_TABLE(const Grammar &g, ProductionVecConstRef lr_items, const std::vector<ProductionVec> &C) {
  return LR_PARSE_TABLE(g, lr_items, C, [&](auto &table, auto state, auto &item) {
    for (const auto &t : g.V_T) {
      table[{state, t}].insert("r" + std::to_string(item.index));
    }
  });
}

LR0ParseTable SLR_PARSE_TABLE(const Grammar &g, ProductionVecConstRef lr_items, const std::vector<ProductionVec> &C, const FOLLOWS_Type &follows) {
  return LR_PARSE_TABLE(g, lr_items, C, [&](auto &table, auto state, auto &item) {
    if (follows.count(item.l)) {
      auto &fs = follows.at(item.l);
      for (const auto &t : fs) {
        table[{state, t}].insert("r" + std::to_string(item.index));
      }
    }
  });
}

LR0ParseTable LL1_PARSE_TABLE(const Grammar &g, ProductionVecConstRef lr_items, const std::vector<ProductionVec> &C) {
  return LR_PARSE_TABLE(g, lr_items, C, [&](auto &table, auto state, auto &item) {
    table[{state, item.lookahead}].insert("r" + std::to_string(item.index));
  });
}

// LL1 CLOSURE
ProductionVec CLOSURE(const Grammar &g, const FIRSTS_Type &firsts, ProductionVecConstRef lr_items, ProductionVec I) {
  size_t last_count = -1;
  // 重复直到没有新的状态
  for (; I.size() != last_count;) {
    last_count = I.size();
    // 遍历每个状态
    for (int i = 0; i < I.size(); ++i) {
      // 遍历每个下一个符号是非终结符的项目
      if (is_next_symbol_non_terminal(g, I[i])) {
        auto nnt = next_symbol(I[i]);
        auto nss = next_symbols(I[i], I[i].lookahead);
        nss.erase(nss.begin()); // 去除串首非终结符

        // 遍历每个等价项目
        for (auto p2 : lr_items) {
          auto &p = I[i];
          if (nnt == p2.l && is_move_in_state(p2)) {
            // 获取FIRST集合
            auto fs = FIRST(g, firsts, nss);
            // 遍历FIRST集合中的每个元素
            for (const auto &f : fs) {
              p2.lookahead = f;
              // 加入I中
              if (std::find(I.begin(), I.end(), p2) == I.end()) {
                I.push_back(p2);
              }
            }
          }
        }
      }
    }
  }

  return std::move(I);
}

// LL1 GOTO
ProductionVec GOTO(const Grammar &g, const FIRSTS_Type &firsts, ProductionVecConstRef lr_items, ProductionVecConstRef I, const std::string &X) {
  ProductionVec J;
  for (const auto &item : I) {
    if (is_next_symbol_X(item, X)) {
      J.push_back(next_state(item));
    }
  }
  return CLOSURE(g, firsts, lr_items, J);
}

/// 求规范LR1项集簇
std::vector<ProductionVec> CANONICAL_LR1_COLLECTION(const Grammar &g, const FIRSTS_Type &firsts, ProductionVec lr_items) {
  Production lr_item_S_move_in = {"S'", {LR_PLACE_HOLDER, g.S}};
  Production lr_item_S_reduce = {"S'", {g.S, LR_PLACE_HOLDER}};
  lr_items.push_back(lr_item_S_move_in);
  lr_items.push_back(lr_item_S_reduce);
  // 与LR0不同之处
  lr_item_S_move_in.lookahead = "$";
  std::vector<ProductionVec> C = {CLOSURE(g, firsts, lr_items, {lr_item_S_move_in})};
  auto goto_func = [&](const Grammar &g, ProductionVecConstRef lr_items, ProductionVecConstRef I, const std::string &X) {
    return GOTO(g, firsts, lr_items, I, X);
  };
  return std::move(CANONICAL_COLLECTION(g, lr_items, C, goto_func));
}

} // namespace lr

GrammarAnalyze analyze(Grammar &g, bool analyze_lr1 = false) {
  GrammarAnalyze ga;
  ga.firsts = FIRSTS(g);
  ga.follows = FOLLOWS(g, ga.firsts);
  ga.selects = SELECTS(g, ga.firsts, ga.follows);
  ga.lr_items = lr::LR_ITEMS(g);
  ga.lr0_C = lr::CANONICAL_LR0_COLLECTION(g, ga.lr_items);
  ga.lr0_parse_table = lr::LR0_PARSE_TABLE(g, ga.lr_items, ga.lr0_C);
  ga.slr_parse_table = lr::SLR_PARSE_TABLE(g, ga.lr_items, ga.lr0_C, ga.follows);
  if (analyze_lr1) {
    ga.lr1_C = lr::CANONICAL_LR1_COLLECTION(g, ga.firsts, ga.lr_items);
    ga.lr1_parse_table = lr::LL1_PARSE_TABLE(g, ga.lr_items, ga.lr1_C);
  }
  return std::move(ga);
}

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

void print_FIRSTS(const Grammar &g, const FIRSTS_Type &firsts) {
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

void print_FOLLOWS(const Grammar &g, const FOLLOWS_Type &firsts) {
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

void print_SELECT(const Production &p, const SELECTS_Type::mapped_type &ss) {
  auto &l = p.l;
  auto &rs = p.r;
  std::cout << "SELECT(" << p << ")= ";
  for (const auto &f : ss) {
    std::cout << f << ' ';
  }
  std::cout << std::endl;
}

void print_SELECTS(const Grammar &g, const SELECTS_Type &selects) {
  std::cout << "SELECT(X): " << std::endl;
  for (const auto &p : g.P) {
    if (selects.count(p)) {
      print_SELECT(p, selects.at(p));
    }
  }
  std::cout << std::endl;
}

void print_LL1_CHECK(const Grammar &g, const SELECTS_Type &selects) {
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

void print_LR_ParseTable(const Grammar &g, const std::vector<ProductionVec> &C, const LR0ParseTable &table) {
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
        auto &table_cell = table.at({i, t});
        does = boost::join(table_cell, "/");
      }
      std::cout << does << "\t";
    }
    for (const auto &n : V_N) {
      std::string does;
      if (table.count({i, n})) {
        auto &table_cell = table.at({i, n});
        does = boost::join(table_cell, "/");
      }
      std::cout << does << "\t";
    }
    std::cout << std::endl;
  }
}

ProductionVec simple_ps = {
    {"Expr", {"Term1"}},
    {"Term", {"Id"}},
    {"Term1", {"Term"}},
    {"Term1", {"Term1", "+", "Term1"}},
};

ProductionVec ps = {
    {"Expr", {"Term15"}},
    {"Term", {"Id"}},
    {"Term", {"IntegerLiteral"}},
    {"Term", {"FloatingLiteral"}},
    {"Term", {"StringLiteral"}},
    {"Term", {"Id", "(", "Term15", ")"}},
    {"Term1", {"Term"}},
    {"Term1", {"Term1", ".", "Id"}},
    {"Term1", {"Term1", "->", "Id"}},
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

} // namespace

int main() {
  auto is_terminal = [](const std::string &x) {
    std::set<std::string> terminals = {"Id", "IntegerLiteral", "FloatingLiteral", "StringLiteral"};
    return !std::isalpha(x[0]) || !std::isupper(x[0]) || terminals.find(x) != terminals.end();
  };
  Grammar g = create(ps, "Expr", {"$"}, "e", is_terminal);

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

  auto ga = analyze(g);
  print_FIRSTS(g, ga.firsts);
  print_FOLLOWS(g, ga.follows);
  print_SELECTS(g, ga.selects);
  print_Productions(ga.lr_items);
  print_Productions(lr::CLOSURE(g, ga.lr_items, {ga.lr_items[0]}));
  print_LL1_CHECK(g, ga.selects);
  print_LR_C(g, ga.lr0_C);
  print_LR_ParseTable(g, ga.lr0_C, ga.lr0_parse_table);
  print_LR_ParseTable(g, ga.lr0_C, ga.slr_parse_table);

  /// 打印结果

  // auto print_firsts = [&](std::ostream &os, const Production &p) {
  //   for (const auto &t : firsts[p.l]) {
  //     os << t << ' ';
  //   }
  //   return "";
  // };
  // auto print_follows = [&](std::ostream &os, const Production &p) {
  //   for (const auto &t : follows[p.l]) {
  //     os << t << ' ';
  //   }
  //   return "";
  // };
  //
  // std::ofstream ofs("FIRST-FOLLOW-SELECT.md");
  // ofs << "|非终结符|FIRST|FOLLOW|\n";
  // ofs << "|--------|-----|------|\n";
  // for (auto &p : ps) {
  //   ofs << "|" << p.l << "|" << print_firsts(ofs, p) << "|" << print_follows(ofs, p) << "|\n";
  // }
  return 0;
}