#pragma once

#include <iterator>
#include <utility>
#include "grammar.hpp"

class Earley {
 private:
  GrammarFree gr;
  Rule start_rule;
  static constexpr char new_start = '@';

  struct State {
    Rule rule;
    size_t dot_pos;
    size_t left_pos;
    bool operator<(const State& another) const;
  };
  friend std::ostream& operator<<(std::ostream& out, const State& st);

  static std::set<State> Scan(const std::set<State>& exist, char cur_letter,
                              std::set<State>& new_exist);

  std::set<State> Predict(const std::set<State>& layer_by_r,
                          std::set<State>& exist, size_t cur_r) const;

  std::set<State> Complete(const std::set<State>& layer_r,
                           std::vector<std::set<State>>& exists,
                           size_t cur_r) const;

 public:
  Earley() = default;

  Earley(const GrammarFree& another_gr);

  void fit(const GrammarFree& gr);

  bool predict(const std::string& word) const;
};