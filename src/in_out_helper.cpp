#include "in_out_helper.hpp"

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

std::string ScanString(bool is_scan_empty) {
  std::string str;
  do {
    std::getline(std::cin, str);
    NormalizeString(str);
  } while (str.empty() && !is_scan_empty);
  return str;
}
