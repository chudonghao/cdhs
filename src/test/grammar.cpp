//
// Created by chudonghao on 2022/1/30.
//

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct ProducerEquation {
  std::string l;
  std::vector<std::string> r;
};

std::vector<ProducerEquation> pes = {
    {"S", {"e"}},
    {"S", {"Primary", "S"}},
    {"Primary", {"Sentence"}},
    {"Sentence", {"Expr", ";"}},
    {"Sentence", {"if", "(", "Expr", ")", "Sentence_1"}},
    {"Sentence", {"for", "(", ";", "Expr", ";", "Expr)", "Sentence_1"}},
    {"Sentence_1", {"{", "Sentences", "}"}},
    {"Sentence_1", {"Sentence"}},
    {"Sentences", {"e"}},
    {"Sentences", {"Sentence", "Sentences"}},
    {"Expr", {"Term15"}},
    {"Term", {"Id", "Term_1"}},
    {"Term", {"ConstNumber"}},
    {"Term_1", {"e"}},
    {"Term_1", {"(", "Term15", ")"}},
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

bool is_terminal(const std::string &x) { return !std::isalpha(x[0]) || !std::isupper(x[0]) || x == "Id" || x == "ConstNumber"; }

int main() {
  std::map<std::string, std::set<std::string>> firsts;
  std::map<std::string, std::set<std::string>> follows;

  size_t count = 0;
  size_t last_count = -1;
  for (; count != last_count;) {

    // 重试
    last_count = count;
    count = 0;

    // 获取所有first集合
    for (auto &item : pes) {
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
          for (const auto &f : fs2) {
            fs.insert(f);
          }
          break;
        }
      }
      count += fs.size();
    }
  }

  for (const auto &item : firsts) {
    auto l = item.first;
    auto &fs = item.second;
    std::cout << l << ' ';
    for (const auto &f : fs) {
      std::cout << f << ' ';
    }
    std::cout << std::endl;
  }
  return 0;
}