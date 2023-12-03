#pragma once

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "in_out_helper.hpp"

struct Grammar {
  using Rule = std::pair<char, std::string>;
  std::set<char> non_terms;
  std::set<char> alph;
  std::set<Rule> rules;
  char start;
  std::map<char, std::vector<std::string>> fast_rules;

  Grammar() = default;
  Grammar(const std::set<char>& non_terms, const std::set<char>& alph,
          const std::set<Rule>& rules, char start)
      : non_terms(non_terms), alph(alph), rules(rules), start(start) {
    for (const auto& rule : rules) {
      fast_rules[rule.first].push_back(rule.second);
    }
  }
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

  temp = ScanString(false);
  std::set<char> non_terms(temp.begin(), temp.end());

  temp = ScanString(false);
  std::set<char> alph(temp.begin(), temp.end());

  std::set<Grammar::Rule> rules;
  for (size_t i = 0; i < cnt_of_rules; ++i) {
    temp = ScanString(false);
    rules.insert(GetRule(temp));
  }

  temp = ScanString(false);
  char start = temp[0];

  gr = Grammar(non_terms, alph, rules, start);
  return in;
}

std::ostream& operator<<(std::ostream& out, const Grammar& gr) {
  out << "\nStart Print Grammar\n";

  out << gr.non_terms.size() << ' ' << gr.alph.size() << ' ' << gr.rules.size()
      << '\n';

  Print(gr.non_terms.begin(), gr.non_terms.end(), out);
  Print(gr.alph.begin(), gr.alph.end(), out);

  for (const auto& rule : gr.rules) {
    out << rule.first << " -> " << rule.second << '\n';
  }

  out << gr.start << '\n';
  out << "End Print Grammar\n" << std::endl;
  return out;
}