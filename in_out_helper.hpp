#pragma once

#include <iostream>
#include <string>
#include <utility>

void NormalizeString(std::string& str) {
  std::string new_str;
  new_str.reserve(str.size());
  for (char ch : str) {
    if (!std::isspace(ch)) {
      new_str.push_back(ch);
    }
  }
  str = std::move(new_str);
}

std::string ScanString() {
  std::string str;
  while (str.empty()) {
    std::getline(std::cin, str);
    NormalizeString(str);
  }
  return str;
}

template <typename Iterator, typename OutStream>
void Print(Iterator begin_it, Iterator end_it, OutStream& out) {
  for(Iterator it = begin_it; it != end_it; ++it) {
    out << *it << ' ';
  }
  out << '\n';
}