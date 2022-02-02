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

struct Production {
  std::string l;
  std::vector<std::string> r;
};

bool operator<(const Production &l, const Production &r) {
  if (l.l < r.l) {
    return true;
  } else if (l.l == r.l) {
    return l.r < r.r;
  } else {
    return false;
  }
}

bool operator==(const Production &l, const Production &r) { return l.l == r.l && l.r == r.r; }
bool operator!=(const Production &l, const Production &r) { return !(l == r); }

std::ostream &operator<<(std::ostream &os, const Production &p) {
  os << p.l << " ->";
  for (const auto &r : p.r) {
    os << " " << r;
  }
  return os;
}

std::vector<Production> ps = {
    {"Expr", {"Term15"}},
    {"Term", {"Id", "Term_1"}},
    {"Term", {"IntegerLiteral"}},
    {"Term", {"FloatingLiteral"}},
    {"Term", {"StringLiteral"}},
    {"Term_1", {"(", "Term15", ")"}},
    {"Term_1", {"e"}},
    {"Term1_b", {"Term"}},
    {"Term1_b", {"(", "Term14", ")"}},
    {"Term1_a", {"Term1_1"}},
    {"Term1_1", {".", "Id"}},
    {"Term1_1", {"->", "Id"}},
    {"Term1_1", {"[", "Term14", "]"}},
    {"Term1", {"Term1_b", "Term1_A1"}},
    {"Term1_A1", {"Term1_a", "Term1_A1"}},
    {"Term1_A1", {"e"}},
    {"Term2_b", {"Term1"}},
    {"Term2_b", {"!", "Term2"}},
    {"Term2_b", {"-", "Term2"}},
    {"Term2_b", {"~", "Term2"}},
    {"Term2_b", {"++", "Term2"}},
    {"Term2_b", {"--", "Term2"}},
    {"Term2_b", {"*", "Term2"}},
    {"Term2_b", {"&", "Term2"}},
    {"Term2_a", {"++"}},
    {"Term2_a", {"--"}},
    {"Term2", {"Term2_b", "Term2_A1"}},
    {"Term2_A1", {"Term2_a", "Term2_A1"}},
    {"Term2_A1", {"e"}},
    {"Term3_b", {"Term2"}},
    {"Term3_a", {"*", "Term3"}},
    {"Term3_a", {"/", "Term3"}},
    {"Term3_a", {"%", "Term3"}},
    {"Term3", {"Term3_b", "Term3_A1"}},
    {"Term3_A1", {"Term3_a", "Term3_A1"}},
    {"Term3_A1", {"e"}},
    {"Term4_b", {"Term3"}},
    {"Term4_a", {"+", "Term4"}},
    {"Term4_a", {"-", "Term4"}},
    {"Term4", {"Term4_b", "Term4_A1"}},
    {"Term4_A1", {"Term4_a", "Term4_A1"}},
    {"Term4_A1", {"e"}},
    {"Term5", {"Term4"}},
    {"Term6_b", {"Term5"}},
    {"Term6_a", {">", "Term6"}},
    {"Term6_a", {">=", "Term6"}},
    {"Term6_a", {"<", "Term6"}},
    {"Term6_a", {"<=", "Term6"}},
    {"Term6", {"Term6_b", "Term6_A1"}},
    {"Term6_A1", {"Term6_a", "Term6_A1"}},
    {"Term6_A1", {"e"}},
    {"Term7_b", {"Term6"}},
    {"Term7_a", {"==", "Term7"}},
    {"Term7_a", {"!=", "Term7"}},
    {"Term7", {"Term7_b", "Term7_A1"}},
    {"Term7_A1", {"Term7_a", "Term7_A1"}},
    {"Term7_A1", {"e"}},
    {"Term8_b", {"Term7"}},
    {"Term8_a", {"&", "Term8"}},
    {"Term8", {"Term8_b", "Term8_A1"}},
    {"Term8_A1", {"Term8_a", "Term8_A1"}},
    {"Term8_A1", {"e"}},
    {"Term9_b", {"Term8"}},
    {"Term9_a", {"^", "Term9"}},
    {"Term9", {"Term9_b", "Term9_A1"}},
    {"Term9_A1", {"Term9_a", "Term9_A1"}},
    {"Term9_A1", {"e"}},
    {"Term10_b", {"Term9"}},
    {"Term10_a", {"|", "Term10"}},
    {"Term10", {"Term10_b", "Term10_A1"}},
    {"Term10_A1", {"Term10_a", "Term10_A1"}},
    {"Term10_A1", {"e"}},
    {"Term11_b", {"Term10"}},
    {"Term11_a", {"&&", "Term11"}},
    {"Term11", {"Term11_b", "Term11_A1"}},
    {"Term11_A1", {"Term11_a", "Term10_A1"}},
    {"Term11_A1", {"e"}},
    {"Term12_b", {"Term11"}},
    {"Term12_a", {"||", "Term12"}},
    {"Term12", {"Term12_b", "Term12_A1"}},
    {"Term12_A1", {"Term12_a", "Term12_A1"}},
    {"Term12_A1", {"e"}},
    {"Term13_b", {"Term12"}},
    {"Term13_a", {"?", "Term13", ":", "Term13"}},
    {"Term13", {"Term13_b", "Term13_A1"}},
    {"Term13_A1", {"Term13_a", "Term13_A1"}},
    {"Term13_A1", {"e"}},
    {"Term14_b", {"Term13"}},
    {"Term14_a", {"=", "Term14"}},
    {"Term14", {"Term14_b", "Term14_A1"}},
    {"Term14_A1", {"Term14_a", "Term14_A1"}},
    {"Term14_A1", {"e"}},
    {"Term15_b", {"Term14"}},
    {"Term15_a", {",", "Term15"}},
    {"Term15", {"Term15_b", "Term15_A1"}},
    {"Term15_A1", {"Term15_a", "Term15_A1"}},
    {"Term15_A1", {"e"}},
};

bool is_terminal(const std::string &x) {
  std::set<std::string> terminals = {"Id", "IntegerLiteral", "FloatingLiteral", "StringLiteral"};
  return !std::isalpha(x[0]) || !std::isupper(x[0]) || terminals.find(x) != terminals.end();
}

int main() {
  std::map<std::string, std::set<std::string>> firsts;
  std::map<std::string, std::set<std::string>> follows;
  std::map<Production, std::set<std::string>> selects;

  /// 计算所有FIRST集
  size_t count = 0;
  size_t last_count = -1;
  for (; count != last_count;) {

    // 重试
    last_count = count;
    count = 0;

    // 更新所有first集合
    for (auto &item : ps) {
      auto &l = item.l;
      auto &rs = item.r;
      auto &fs = firsts[l];

      for (const auto &r : rs) {
        // 如果元素是终结符且不是e，则将其加入到first集中，跳出循环
        // 如果是非终结符，则该非终结符的所有first集的所有内容加入first集中
        if (is_terminal(r)) {
          fs.insert(r);
          if (r != "e") {
            break;
          }
        } else {
          //
          auto &fs2 = firsts[r];
          fs.insert(fs2.begin(), fs2.end());
          break;
        }
      }
      count += fs.size();
    }
  }

  /// 计算所有FOLLOW集
  count = 0;
  last_count = -1;

  // 手动添加Expr的FOLLOW集
  // follows["Expr"].insert(); // 继承 Term15 empty
  follows["Expr"].insert(")"); // 继承 ValueStmt
  follows["Expr"].insert(";"); // 继承 Stmt

  for (; count != last_count;) {

    // 重试
    last_count = count;
    count = 0;

    // 更新所有follow集合
    for (auto &item : ps) {
      auto &l = item.l;
      auto &rs = item.r;

      // 对每个产生式的右部的每个非终结符，计算其FOLLOW集
      for (int i = 0; i < rs.size(); ++i) {
        auto &r = rs[i];
        // 跳过终结符
        if (is_terminal(r)) {
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
          if (is_terminal(f)) {
            fs.insert(f);
          } else {
            auto &fs2 = firsts[f];
            std::for_each(fs2.begin(), fs2.end(), [&](auto &v) {
              // 如果FIRST集有空，则移到下一个元素并继续当前过程
              // 如果不是空，则加入到FOLLOW集中
              if (v == "e") {
                encounter_empty = true;
              } else {
                fs.insert(v);
              }
            });
          }
        }
      }
    }

    std::for_each(follows.begin(), follows.end(), [&](auto &p) { count += p.second.size(); });
  }

  /// 计算所有SELECT集
  for (auto &p : ps) {
    auto &ss = selects[p];
    // 计算右部的FIRST集
    std::set<std::string> fs;
    auto &rs = p.r;
    for (const auto &r : rs) {
      // 如果遇到终结符，则停止计算
      if (is_terminal(r)) {
        fs.insert(r);
        break;
      }
      auto fs2 = firsts[r];
      fs.insert(fs2.begin(), fs2.end());
      // 如果FIRST集合不包含空，则停止计算
      if (fs2.count("e") == 0) {
        break;
      }
    }

    // 计算SELECT集
    if (fs.count("e") == 0) {
      ss.insert(fs.begin(), fs.end());
    } else {
      auto fs2 = follows[p.l];
      ss.insert(fs.begin(), fs.end());
      ss.insert(fs2.begin(), fs2.end());
      ss.erase("e");
    }
  }

  /// 打印结果
  std::set<std::string> viewed;
  std::cout << "FIRST(X): " << std::endl;
  for (const auto &p : ps) {
    auto &l = p.l;
    auto &fs = firsts[l];
    if (viewed.count(l)) {
      continue;
    } else {
      viewed.insert(l);
    }
    std::cout << "FIRST(" + l + ")= ";
    for (const auto &f : fs) {
      std::cout << f << ' ';
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  viewed.clear();

  std::cout << "FOLLOW(X): " << std::endl;
  for (const auto &p : ps) {
    auto &l = p.l;
    auto &fs = follows[l];
    if (viewed.count(l)) {
      continue;
    } else {
      viewed.insert(l);
    }
    std::cout << "FOLLOW(" << l << ")= ";
    for (const auto &f : fs) {
      std::cout << f << ' ';
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  viewed.clear();

  auto print_selects = [&](const Production &p) {
    auto &l = p.l;
    auto &rs = p.r;
    auto &ss = selects[p];
    std::cout << "SELECT(" << p << ") <==== ";
    for (const auto &f : ss) {
      std::cout << f << ' ';
    }
    std::cout << std::endl;
  };
  std::cout << "SELECT(X): " << std::endl;
  for (const auto &p : ps) {
    print_selects(p);
  }
  std::cout << std::endl;

  std::cout << "LL1 CHECK: " << std::endl;
  for (auto iter = ps.begin(); iter != ps.end(); ++iter) {
    auto &p = *iter;
    auto &l = p.l;
    auto &ss = selects[p];
    if (viewed.count(l)) {
      continue;
    } else {
      viewed.insert(l);
    }

    for (auto iter2 = std::next(iter); iter2 != ps.end(); ++iter2) {
      auto &p2 = *iter2;
      auto &l2 = p2.l;
      auto &ss2 = selects[p2];
      if (l2 == l) {
        std::vector<std::string> iss;
        std::set_intersection(ss.begin(), ss.end(), ss2.begin(), ss2.end(), std::back_inserter(iss));
        if (!iss.empty()) {
          std::cout << "LL1 CHECK FAILED: " << std::endl;
          print_selects(p);
          print_selects(p2);
        }
      }
    }
  }

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