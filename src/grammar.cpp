#include "grammar.hpp"

#include <cctype>
#include <stdexcept>
#include <utility>

std::ostream& operator<<(std::ostream& out, const Rule& rule) {
  std::cout << rule.first << " -> " << rule.second;
  return out;
}

GrammarFree::GrammarFree(const std::set<char>& non_terms,
                         const std::set<char>& alph,
                         const std::set<Rule>& rules, char start)
    : non_terms(non_terms), alph(alph), rules(rules), start(start) {
  for (const auto& rule : rules) {
    fast_rules[rule.first].push_back(rule.second);
  }
}

std::pair<char, std::string> GetRule(std::string& str) {
  return {str[0], str.substr(3)};
}

template <typename T>
void CheckSize(const T& str, size_t req_size, std::string message) {
  if (str.size() != req_size) {
    throw std::runtime_error("bad input - bad size - " + std::move(message));
  }
}

bool IsNonTerm(char ch) { return std::isupper(ch); }

template <typename T>
void CheckNonTerms(const T& str, size_t req_size) {
  CheckSize(str, req_size, "non terms");

  for (char ch : str) {
    if (!IsNonTerm(ch)) {
      throw std::runtime_error("bad input - non terms");
    }
  }
}

bool IsAlph(char ch) {
  static const std::set<char> extra = {'(', ')', '{', '}', '[', ']',
                                       '*', '/', '%', '+', '-'};
  return std::islower(ch) || std::isdigit(ch) || extra.find(ch) != extra.end();
}

template <typename T>
void CheckAlph(const T& str, size_t req_size) {
  CheckSize(str, req_size, "alph");

  for (char ch : str) {
    if (!IsAlph(ch)) {
      throw std::runtime_error("bad input - alph");
    }
  }
}

void CheckRule(const std::string& str) {
  static const std::string exc = "bad input - rule - ";

  if (str.size() < 3) {
    throw std::runtime_error(exc + "small size");
  }

  if (!IsNonTerm(str[0])) {
    throw std::runtime_error(exc + "left - not a non term");
  }

  if (str.substr(1, 2) != "->") {
    throw std::runtime_error(exc + "not a ->");
  }

  for (size_t i = 3; i < str.size(); ++i) {
    if (!IsAlph(str[i]) && !IsNonTerm(str[i])) {
      throw std::runtime_error(exc + "right - bad symb");
    }
  }
}

void CheckStart(const std::string& str) {
  CheckSize(str, 1, "Start");

  if (!IsNonTerm(str[0])) {
    throw std::runtime_error("bad Start symb");
  }
}

std::istream& operator>>(std::istream& in, GrammarFree& gr) {
  size_t cnt_of_non_terms;
  size_t size_of_alph;
  size_t cnt_of_rules;
  std::cin >> cnt_of_non_terms >> size_of_alph >> cnt_of_rules;

  std::string temp;

  temp = ScanString(false);
  std::set<char> non_terms(temp.begin(), temp.end());
  CheckNonTerms(non_terms, cnt_of_non_terms);

  temp = ScanString(false);
  std::set<char> alph(temp.begin(), temp.end());
  CheckAlph(alph, size_of_alph);

  std::set<Rule> rules;
  for (size_t i = 0; i < cnt_of_rules; ++i) {
    temp = ScanString(false);
    CheckRule(temp);
    rules.insert(GetRule(temp));
  }

  temp = ScanString(false);
  CheckStart(temp);
  char start = temp[0];

  gr = GrammarFree(non_terms, alph, rules, start);
  return in;
}

std::ostream& operator<<(std::ostream& out, const GrammarFree& gr) {
  out << std::endl;
  out << "Start Print GrammarFree\n";

  out << gr.non_terms.size() << ' ' << gr.alph.size() << ' ' << gr.rules.size()
      << '\n';

  Print(gr.non_terms.begin(), gr.non_terms.end(), out);
  Print(gr.alph.begin(), gr.alph.end(), out);

  for (const auto& rule : gr.rules) {
    out << rule << std::endl;
  }

  out << gr.start << '\n';
  out << "End Print GrammarFree\n" << std::endl;
  return out;
}