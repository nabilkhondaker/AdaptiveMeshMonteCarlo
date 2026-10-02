#include "ammc/core/version.hpp"
#include <iostream>
int main() {
  std::cout << "AMMC uncertainty tool v" << ammc::kVersion << " (author: " << ammc::kAuthor << ")\n";
  std::cout << "See documentation for usage.\n";
  return 0;
}
