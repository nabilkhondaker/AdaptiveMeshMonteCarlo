#include <iostream>
extern int test_rng();
extern int test_cholesky();
extern int test_online_stats();
extern int test_bs_validation();
int main() {
  int fails = 0;
  fails += test_rng();
  fails += test_cholesky();
  fails += test_online_stats();
  fails += test_bs_validation();
  if (fails == 0) std::cout << "ALL UNIT TESTS PASSED\n";
  else std::cout << "FAILURES: " << fails << "\n";
  return fails;
}
