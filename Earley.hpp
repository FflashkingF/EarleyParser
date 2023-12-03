#pragma once

#include <iterator>

#include "grammar.hpp"

class Earley {
 private:
  Grammar gr;
  Grammar::Rule start_rule;

  struct State {
    Grammar::Rule rule;
    size_t dot_pos;
    size_t left_pos;
    bool operator<(const State& another) const {
      if (rule < another.rule) {
        return true;
      }
      return rule == another.rule &&
             (dot_pos < another.dot_pos ||
              (dot_pos == another.dot_pos && left_pos < another.left_pos));
    }
  };

  static std::vector<State> Scan(const std::set<State>& used, char cur_letter) {
    std::vector<State> new_layer_by_r;
    new_layer_by_r.reserve(used.size());

    for (const State& st : used) {
      if (st.rule.second[st.dot_pos] == cur_letter) {
        new_layer_by_r.push_back({st.rule, st.dot_pos + 1, st.left_pos});
      }
    }

    return new_layer_by_r;
  }

  std::vector<State> Predict(const std::vector<State>& layer_by_r,
                             const std::set<State>& used, size_t cur_r) const {
    std::vector<State> new_states;

    for (const State& st : layer_by_r) {
      char cur_symb = st.rule.second[st.dot_pos];

      if (!gr.non_terms.contains(cur_symb)) {
        continue;
      }

      auto it = gr.fast_rules.find(cur_symb);
      if (it == gr.fast_rules.end()) continue;

      for (const auto& right_rule : it->second) {
        State new_state = State{Grammar::Rule{cur_symb, right_rule}, 0, cur_r};

        if (!used.contains(new_state)) {
          new_states.push_back(new_state);
        }
      }
    }

    return new_states;
  }

  // static std::vector<State> Complete(const std::vector<State>& layer_by_r,
  //                                    const std::set<State>& used) {
  //   std::vector<State> new_states;
  //   for (const State& st : layer_by_r) {
  //   }
  // }

 public:
  Earley(const Grammar& another_gr)
      : gr(another_gr),
        start_rule(Grammar::Rule{' ', std::string(1, gr.start)}) {
    gr.non_terms.insert(' ');
    gr.rules.insert(start_rule);
    gr.fast_rules[' '].push_back(start_rule.second);
    gr.start = ' ';
  }

  void fit(const Grammar& gr) {
    Earley copy(gr);
    std::swap(*this, copy);
  }

  bool predict(const std::string& word) const {
    std::vector<State> layer_by_r = {State{start_rule, 0, 0}};
    std::set<State> used;

    for (size_t cur_r = 0; cur_r <= word.size(); ++cur_r) {
      if (cur_r != 0) {
        layer_by_r = Scan(used, word[cur_r]);
        used.clear();
      }

      while (!layer_by_r.empty()) {
        // auto&& new_el_comp = Complete(layer_by_r, used, cur_r);
        layer_by_r = Predict(layer_by_r, used, cur_r);
      }
    }

    return used.contains(State{start_rule, 1, 0});
  }
};