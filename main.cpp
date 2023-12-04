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
  std::cout << gr;
  Earley ea(gr);
  size_t q;
  std::cin >> q;
  std::string word;
  while(q-- != 0) {
    std::cin >> word;
    std::cout << (ea.predict(word) ? "YES" : "NO" ) << std::endl << std::endl << std::endl;;
  }
}
