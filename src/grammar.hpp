#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "in_out_helper.hpp"

using Rule = std::pair<char, std::string>;
std::ostream& operator<<(std::ostream& out, const Rule& rule);

struct GrammarFree {
  std::set<char> non_terms;
  std::set<char> alph;
  std::set<Rule> rules;
  char start;
  std::map<char, std::vector<std::string>> fast_rules;

  GrammarFree() = default;
  GrammarFree(const std::set<char>& non_terms, const std::set<char>& alph,
          const std::set<Rule>& rules, char start);
};

std::pair<char, std::string> GetRule(std::string& str);

std::istream& operator>>(std::istream& in, GrammarFree& gr);

std::ostream& operator<<(std::ostream& out, const GrammarFree& gr);