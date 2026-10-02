#include "ammc/linear_algebra/cholesky.hpp"
#include <cassert>
#include <iostream>
#include <vector>

int test_cholesky() {
  // Identity
  std::vector<double> I = {1,0,0, 0,1,0, 0,0,1};
  ammc::linalg::Cholesky chol(3);
  assert(chol.factorize(I));
  auto d = ammc::linalg::diagnose_correlation(I, 3);
  assert(d.positive_definite);
  // Invalid (not PD)
  std::vector<double> bad = {1,2,0, 2,1,0, 0,0,1};
  ammc::linalg::Cholesky chol2(3);
  assert(!chol2.factorize(bad));
  std::cout << "test_cholesky PASSED\n";
  return 0;
}
