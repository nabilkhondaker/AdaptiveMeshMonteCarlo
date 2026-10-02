#pragma once
#include <cmath>
#include <cstdint>

namespace ammc::stats {

/// Welford online mean / variance accumulator (numerically stable).
class OnlineStats {
 public:
  void add(double x) {
    ++n_;
    double delta = x - mean_;
    mean_ += delta / static_cast<double>(n_);
    double delta2 = x - mean_;
    m2_ += delta * delta2;
  }

  std::uint64_t count() const { return n_; }
  double mean() const { return mean_; }
  double variance() const { return n_ > 1 ? m2_ / static_cast<double>(n_ - 1) : 0.0; }
  double std_dev() const { return std::sqrt(variance()); }
  double std_error() const { return n_ > 0 ? std_dev() / std::sqrt(static_cast<double>(n_)) : 0.0; }

  void reset() { n_ = 0; mean_ = 0; m2_ = 0; }

 private:
  std::uint64_t n_{0};
  double mean_{0.0};
  double m2_{0.0};
};

}  // namespace ammc::stats
