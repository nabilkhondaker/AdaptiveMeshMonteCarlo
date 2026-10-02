#include "ammc/random/rng.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

int test_rng() {
  ammc::random::RngStream a(42, 0, 0);
  ammc::random::RngStream b(42, 0, 0);
  // Same seed/path => identical stream
  for (int i = 0; i < 100; ++i) {
    assert(std::abs(a.uniform01() - b.uniform01()) < 1e-15);
  }
  // Different path => different stream
  ammc::random::RngStream c(42, 1, 0);
  double diff = 0;
  for (int i = 0; i < 10; ++i) diff += std::abs(a.uniform01() - c.uniform01());
  assert(diff > 0.01);
  // Normal mean roughly 0
  ammc::random::RngStream n(123, 0, 0);
  double sum = 0;
  for (int i = 0; i < 10000; ++i) sum += n.normal01();
  double mean = sum / 10000.0;
  assert(std::abs(mean) < 0.05);
  std::cout << "test_rng PASSED\n";
  return 0;
}
