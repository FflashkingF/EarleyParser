#pragma once

#include <iterator>

#include "grammar.hpp"

class Earley {
 private:
  Grammar gr;
  Rule start_rule;
  static constexpr char new_start = '@';

  struct State {
    Rule rule;
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

    friend std::ostream& operator<<(std::ostream& out, const State& st) {
      std::cout << "State: " << st.rule << ' ' << st.dot_pos << ' '
                << st.left_pos;
      return out;
    }
  };

  static std::set<State> Scan(const std::set<State>& exist, char cur_letter,
                              std::set<State>& new_exist) {
    std::set<State> new_layer_by_r;

    std::cout << "Scan: " << std::endl;
    std::cout << "cur_letter: " << cur_letter << std::endl;

    for (const State& st : exist) {
      std::cout << st << std::endl;
      if (st.rule.second[st.dot_pos] == cur_letter) {
        std::cout << "YES" << std::endl;
        State new_state{st.rule, st.dot_pos + 1, st.left_pos};
        std::cout << new_state << std::endl;
        new_layer_by_r.insert(new_state);
        new_exist.insert(new_state);
      } else {
        std::cout << "NO" << std::endl;
      }
    }

    return new_layer_by_r;
  }

  std::set<State> Predict(const std::set<State>& layer_by_r,
                          std::set<State>& exist, size_t cur_r) const {
    std::set<State> new_states;
    std::cout << "Predict: " << std::endl;

    for (const State& st : layer_by_r) {
      std::cout << st << std::endl;
      char cur_symb = st.rule.second[st.dot_pos];

      if (!gr.non_terms.contains(cur_symb)) {
        std::cout << "not a term" << std::endl;
        continue;
      }
      std::cout << "term"<< std::endl;

      auto it = gr.fast_rules.find(cur_symb);
      if (it == gr.fast_rules.end()) {
        std::cout << "no rules" << std::endl;
        continue;
      }

      for (const auto& right_rule : it->second) {
        State new_state = State{Rule{cur_symb, right_rule}, 0, cur_r};
        std::cout << "right_rule: " << right_rule  << std::endl;
        std::cout << "new_state: " << new_state << std::endl;
        if (!exist.contains(new_state)) {
          std::cout << "not exist)!" << std::endl;
          new_states.insert(new_state);
          exist.insert(new_state);
        }
        else {
          std::cout << "exist(" << std::endl;
        }
      }
    }

    return new_states;
  }

  std::set<State> Complete(const std::vector<std::set<State>>& layers,
                           std::vector<std::set<State>>& exists, size_t cur_r) const {
    std::set<State> new_states;
    std::cout << "Complete: " << std::endl;

    for (const State& st : layers[cur_r]) {
      std::cout << st << std::endl;
      if (st.dot_pos < st.rule.second.size()) {
        std::cout << "not end" << std::endl;
        continue;
      }
      std::cout << "end" << std::endl;

      for (const State& another_st : exists[st.left_pos]) {
        std::cout << "another_st: " << another_st << std::endl;
        if (another_st.rule.second[another_st.dot_pos] != st.rule.first) {
          std::cout << "no match" << std::endl;
          continue;
        }
        std::cout << "match" << std::endl;

        State new_state{another_st.rule, another_st.dot_pos + 1,
                        another_st.left_pos};
        std::cout << "new_state: " << new_state << std::endl;
        if (!exists[cur_r].contains(new_state)) {
          std::cout << "not eixist)!" << std::endl;
          new_states.insert(new_state);
          exists[cur_r].insert(new_state);
        }
        else {
          std::cout << "exist(" << std::endl;
        }
      }
    }

    return new_states;
  }

 public:
  Earley() {}

  Earley(const Grammar& another_gr)
      : gr(another_gr), start_rule(Rule{new_start, std::string(1, gr.start)}) {
    gr.non_terms.insert(new_start);
    gr.rules.insert(start_rule);
    gr.fast_rules[new_start].push_back(start_rule.second);
    gr.start = new_start;
    std::cout << gr;
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
        auto&& new_el_comp = Complete(layers, exists, cur_r);
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