#pragma once
#include "ammc/random/rng.hpp"
#include "ammc/stochastic/gbm.hpp"
#include "ammc/statistics/online_stats.hpp"
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace ammc::mc {

struct McConfig {
  std::uint64_t n_paths{100000};
  std::uint64_t seed{42};
  int n_steps{100};
  bool antithetic{false};
  bool control_variate{false};
  double confidence_level{0.95};
  bool deterministic{true};
  int n_threads{1};
};

struct McResult {
  double mean{0.0};
  double variance{0.0};
  double std_dev{0.0};
  double std_error{0.0};
  double ci_low{0.0};
  double ci_high{0.0};
  std::uint64_t n_paths{0};
  double runtime_sec{0.0};
  std::string backend{"cpu"};
};

/// Generic path-dependent Monte Carlo engine.
/// The payoff is supplied as a callable that receives the full path of spots.
class MonteCarloEngine {
 public:
  using PayoffFn = std::function<double(const std::vector<double>& path)>;

  explicit MonteCarloEngine(McConfig cfg);

  McResult price_gbm(const stochastic::GbmParams& model, const PayoffFn& payoff) const;

  McResult price_gbm_european_call(const stochastic::GbmParams& model, double strike) const;

 private:
  McConfig cfg_;
};

}  // namespace ammc::mc
