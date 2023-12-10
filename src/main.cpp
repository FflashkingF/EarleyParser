#include <iostream>
#include <set>
#include <string>

#include "Earley.hpp"
#include "grammar.hpp"
#include "in_out_helper.hpp"

struct Aboba {
  int a;
  int b;
};

int main() {
  Grammar gr;
  std::cin >> gr;
  // std::cout << gr;

  Earley ea(gr);

  size_t cnt_queries;
  std::string word_query;
  std::cin >> cnt_queries;
  std::getline(std::cin, word_query);

  while (cnt_queries-- != 0) {
    word_query = ScanString(true);
    std::cout << (ea.predict(word_query) ? "YES" : "NO") << std::endl
              << std::endl
              << std::endl;
    ;
  }
}
