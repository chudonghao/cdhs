//
// Created by chudonghao on 2022/2/5.
//

#ifndef INC_2022_2_5_BAE7962B29324F599E835E81BDEB44C9_H
#define INC_2022_2_5_BAE7962B29324F599E835E81BDEB44C9_H

#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>

namespace cdhs {

struct LR0Production {
  int l{};              // 左部
  std::vector<int> r{}; // 右部
};

class LR0ParseTable {
public:
  enum ActionType {
    ACTION_TYPE_ERROR,
    ACTION_TYPE_MOVE_IN,
    ACTION_TYPE_REDUCE,
    ACTION_TYPE_GOTO,
    ACTION_TYPE_ACCEPT,
  };
  struct Action {
    Action() = default;
    Action(ActionType type, int d) : type(type), state(d) {}

    ActionType type{};
    union {
      int state{};    // move in/goto
      int production; // reduce
      int error;      // error
    };
    bool operator<(const Action &rhs) const {
      if (type < rhs.type)
        return true;
      if (rhs.type < type)
        return false;
      return state < rhs.state;
    }
    bool operator>(const Action &rhs) const { return rhs < *this; }
    bool operator<=(const Action &rhs) const { return !(rhs < *this); }
    bool operator>=(const Action &rhs) const { return !(*this < rhs); }
  };

  static std::string to_string(const Action &action) {
    switch (action.type) {
    default:
    case ACTION_TYPE_ERROR:
      return "";
    case ACTION_TYPE_MOVE_IN:
      return "s" + std::to_string(action.state);
    case ACTION_TYPE_REDUCE:
      return "r" + std::to_string(action.production);
    case ACTION_TYPE_GOTO:
      return std::to_string(action.state);
    case ACTION_TYPE_ACCEPT:
      return "acc";
    }
  }

  LR0ParseTable();

  // void setInitialState(int state) { m_initial_state = state; }
  //
  // int getInitialState() const { return m_initial_state; }

  int getStateSize() const { return m_state_size; }

  void setStateSize(int state_size) { m_state_size = state_size; }

  void setAction(int state, int symbol, Action action);

  Action getAction(int state, int symbol) const;

  Action operator()(int state, int symbol) const { return getAction(state, symbol); }

private:
  // int m_initial_state{};
  int m_state_size{};

  std::map<std::pair<int, int>, Action> m_actions;
};

class LR0StateMachine {
public:
  virtual ~LR0StateMachine() = default;

  virtual void reset();

  int operator()(int symbol);

protected:
  virtual int onReduced(int production_id, int n_symbols) = 0;

  virtual int onErrored(int state, int symbol) = 0;

  virtual int onMovedIn(int state, int symbol) = 0;

  virtual int onWentTo(int state, int symbol) = 0;

  virtual int onAccepted() = 0;

protected:
  LR0ParseTable m_parse_table;
  std::vector<LR0Production> m_productions;

  // 映射符号ID，用于调试等目的
  std::map<int, std::string> m_index2SymbolMap;
  std::map<std::string, int> m_symbol2IndexMap;

  int toSymbolIndex(const std::string &str, int error = 0);

  std::string toSymbolString(int);

  int getProductionLength(int production_id) const;

  int getProductionLeftSymbol(int production_id) const;

protected:
  std::vector<int> m_state_stack;
  std::vector<int> m_symbol_stack;
};

} // namespace cdhs

#endif // INC_2022_2_5_BAE7962B29324F599E835E81BDEB44C9_H
