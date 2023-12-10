#pragma once

#include <iostream>
#include <string>
#include <utility>

void NormalizeString(std::string& str);

std::string ScanString(bool is_scan_empty);

template <typename Iterator, typename OutStream>
void Print(Iterator begin_it, Iterator end_it, OutStream& out) {
  for (Iterator it = begin_it; it != end_it; ++it) {
    out << *it << ' ';
  }
  out << '\n';
}
