#pragma once
#include <chrono>

namespace ammc::utilities {

class Timer {
 public:
  void start() { start_ = std::chrono::steady_clock::now(); }
  double elapsed() const {
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start_).count();
  }
 private:
  std::chrono::steady_clock::time_point start_;
};

}  // namespace ammc::utilities
