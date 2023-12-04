#include <iostream>
#include <set>
#include <string>

#include "Earley.hpp"
// #include "grammar.hpp"

struct Aboba {
  int a;
  int b;
};

int main() {
  Grammar gr;
  std::cin >> gr;
  //std::cout << gr;

  Earley ea(gr);

  size_t cnt_queries;
  std::cin >> cnt_queries;
  std::string word_query;
  
  while(cnt_queries-- != 0) {
    std::cin >> word_query;
    std::cout << (ea.predict(word_query) ? "YES" : "NO" ) << std::endl << std::endl << std::endl;;
  }
}
