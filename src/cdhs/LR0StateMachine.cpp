//
// Created by chudonghao on 2022/2/5.
//

#include "LR0StateMachine.h"

#include <iostream>
#include <stdexcept>

namespace cdhs {

LR0ParseTable::LR0ParseTable() {}

void LR0ParseTable::setAction(int state, int input, LR0ParseTable::Action action) { m_actions[{state, input}] = action; }

LR0ParseTable::Action LR0ParseTable::getAction(int state, int input) const {
  if (auto iter = m_actions.find({state, input}); iter != m_actions.end()) {
    return iter->second;
  }
  return {};
}

void LR0StateMachine::reset() {
  m_state_stack = {};
  m_symbol_stack = {};
}

int LR0StateMachine::operator()(int symbol) {
  int state = m_state_stack.empty() ? 0 : m_state_stack.back();
  LR0ParseTable::Action action = m_parse_table.getAction(state, symbol);

  // std::cout << "current state: " << state << " ";
  // std::cout << "input: " << toSymbolString(symbol) << "(" << std::to_string(symbol) << ") ";
  // std::cout << "action: " << LR0ParseTable::to_string(action) << std::endl;

  switch (action.type) {
  case LR0ParseTable::ACTION_TYPE_ERROR:
    return onErrored(state, symbol);
  case LR0ParseTable::ACTION_TYPE_MOVE_IN:
    m_state_stack.push_back(action.state);
    m_symbol_stack.push_back(symbol);
    return onMovedIn(action.state, symbol);
  case LR0ParseTable::ACTION_TYPE_REDUCE: {
    int len = getProductionLength(action.production);
    symbol = getProductionLeftSymbol(action.production);
    // reduce
    for (int i = len; i > 0; --i) {
      m_state_stack.pop_back();
      m_symbol_stack.pop_back();
    }
    // push reduced
    m_symbol_stack.push_back(symbol);
    int ret = onReduced(action.production, len);
    // goto
    return ret ? ret : (*this)(symbol);
  }
  case LR0ParseTable::ACTION_TYPE_GOTO:
    m_state_stack.push_back(action.state);
    return onWentTo(action.state, symbol);
  case LR0ParseTable::ACTION_TYPE_ACCEPT:
    return onAccepted();
  }

  return -1;
}

int LR0StateMachine::toSymbolIndex(const std::string &str, int error) {
  if (auto iter = m_symbol2IndexMap.find(str); iter != m_symbol2IndexMap.end()) {
    return iter->second;
  }
  return error;
}

std::string LR0StateMachine::toSymbolString(int index) {
  if (auto iter = m_index2SymbolMap.find(index); iter != m_index2SymbolMap.end()) {
    return iter->second;
  }
  return {};
}

int LR0StateMachine::getProductionLength(int production_id) const {
  if (production_id >= 0 && production_id < m_productions.size()) {
    return m_productions[production_id].r.size();
  }
  throw std::logic_error("unknown production " + std::to_string(production_id));
}

int LR0StateMachine::getProductionLeftSymbol(int production_id) const {
  if (production_id >= 0 && production_id < m_productions.size()) {
    return m_productions[production_id].l;
  }
  throw std::logic_error("unknown production " + std::to_string(production_id));
}

} // namespace cdhs
