#include "ammc/statistics/online_stats.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

int test_online_stats() {
  ammc::stats::OnlineStats s;
  s.add(1); s.add(2); s.add(3);
  assert(s.count() == 3);
  assert(std::abs(s.mean() - 2.0) < 1e-12);
  assert(std::abs(s.variance() - 1.0) < 1e-12);
  std::cout << "test_online_stats PASSED\n";
  return 0;
}
