#pragma once

#include <iostream>
#include <set>
#include <vector>

#include "in_out_helper.hpp"

class Grammar {
 private:
  std::set<char> non_terms_;
  std::set<char> alph_;
  std::set<std::pair<char, std::string>> rules_;
  char start_;

 public:
  Grammar() = default;
  Grammar(const std::set<char>& non_terms, const std::set<char>& alph,
          const std::set<std::pair<char, std::string>>& rules, char start)
      : non_terms_(non_terms), alph_(alph), rules_(rules), start_(start) {}

  friend std::ostream& operator<<(std::ostream&, const Grammar&);
};

std::pair<char, std::string> GetRule(std::string& str) {
  return {str[0], str.substr(3)};
}

std::istream& operator>>(std::istream& in, Grammar& gr) {
  size_t cnt_of_non_terms;
  size_t size_of_alph;
  size_t cnt_of_rules;
  std::cin >> cnt_of_non_terms >> size_of_alph >> cnt_of_rules;

  std::string temp;

  temp = ScanString();
  std::set<char> non_terms(temp.begin(), temp.end());

  temp = ScanString();
  std::set<char> alph(temp.begin(), temp.end());

  std::set<std::pair<char, std::string>> rules;
  for (size_t i = 0; i < cnt_of_rules; ++i) {
    temp = ScanString();
    rules.insert(GetRule(temp));
  }

  temp = ScanString();
  char start = temp[0];

  gr = Grammar(non_terms, alph, rules, start);
  return in;
}

std::ostream& operator<<(std::ostream& out, const Grammar& gr) {
  out << "Print Grammar:\n";

  out << gr.non_terms_.size() << ' ' << gr.alph_.size() << ' '
            << gr.rules_.size() << '\n';

  Print(gr.non_terms_.begin(), gr.non_terms_.end(), out);
  Print(gr.alph_.begin(), gr.alph_.end(), out);

  for(const auto& rule : gr.rules_) {
    out << rule.first << " -> " << rule.second << '\n';
  }

  out << gr.start_ << std::endl;

  return out;
}