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

  static std::set<State> Scan(const std::set<State>& exist, char cur_letter,
                              std::set<State>& new_exist) {
    std::set<State> new_layer_by_r;
    std::cout << "what?" << std::endl;
    for (const State& st : exist) {
      std::cout << "shish " << st.rule.second[st.dot_pos] << std::endl;
      if (st.rule.second[st.dot_pos] == cur_letter) {
        State new_state{st.rule, st.dot_pos + 1, st.left_pos};
        std::cout << "aboaba " << cur_letter << std::endl;
        new_layer_by_r.insert(new_state);
        new_exist.insert(new_state);
      }
    }

    return new_layer_by_r;
  }

  std::set<State> Predict(const std::set<State>& layer_by_r,
                          std::set<State>& exist, size_t cur_r) const {
    std::set<State> new_states;

    for (const State& st : layer_by_r) {
      char cur_symb = st.rule.second[st.dot_pos];

      if (!gr.non_terms.contains(cur_symb)) {
        continue;
      }

      auto it = gr.fast_rules.find(cur_symb);
      if (it == gr.fast_rules.end()) continue;

      for (const auto& right_rule : it->second) {
        State new_state = State{Grammar::Rule{cur_symb, right_rule}, 0, cur_r};

        if (!exist.contains(new_state)) {
          new_states.insert(new_state);
          exist.insert(new_state);
        }
      }
    }

    return new_states;
  }

  std::set<State> Complete(const std::vector<std::set<State>>& layers,
                           std::set<State>& exist, size_t cur_r) const {
    std::set<State> new_states;

    for (const State& st : layers[cur_r]) {
      if (st.dot_pos < st.rule.second.size()) {
        continue;
      }

      for (const State& another_st : layers[st.left_pos]) {
        if (another_st.rule.second[another_st.dot_pos] != st.rule.first) {
          continue;
        }

        State new_state{another_st.rule, another_st.dot_pos + 1,
                        another_st.left_pos};
        if (!exist.contains(new_state)) {
          new_states.insert(new_state);
          exist.insert(new_state);
        }
      }
    }

    return new_states;
  }

 public:
  Earley() {}

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
    std::vector<std::set<State>> layers(word.size() + 1);
    std::vector<std::set<State>> exists(word.size() + 1);
    layers[0].insert({start_rule, 0, 0});
    exists[0].insert({start_rule, 0, 0});

    for (size_t cur_r = 0; cur_r <= word.size(); ++cur_r) {
      if (cur_r != 0) {
        layers[cur_r] = Scan(exists[cur_r - 1], word[cur_r - 1], exists[cur_r]);
      }

      while (!layers[cur_r].empty()) {
        auto&& new_el_comp = Complete(layers, exists[cur_r], cur_r);
        auto&& new_el_predict = Predict(layers[cur_r], exists[cur_r], cur_r);
        layers[cur_r] = new_el_comp;
        layers[cur_r].insert(new_el_predict.begin(), new_el_predict.end());
      }
    }
    std::cout << exists.back().size() << std::endl;
    std::cout << exists.back().begin()->rule.second << std::endl;
    return exists.back().contains(State{start_rule, 1, 0});
  }
};