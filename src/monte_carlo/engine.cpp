#include "ammc/monte_carlo/engine.hpp"
#include "ammc/utilities/timer.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

namespace ammc::mc {

MonteCarloEngine::MonteCarloEngine(McConfig cfg) : cfg_(cfg) {}

McResult MonteCarloEngine::price_gbm(const stochastic::GbmParams& model, const PayoffFn& payoff) const {
  utilities::Timer timer;
  timer.start();

  stochastic::GbmProcess process(model);
  const double dt = model.maturity / static_cast<double>(cfg_.n_steps);
  const double disc = std::exp(-model.rate * model.maturity);

  stats::OnlineStats acc;
  std::vector<double> path(cfg_.n_steps + 1);
  const std::uint64_t n = cfg_.n_paths;
  const bool anti = cfg_.antithetic;

  for (std::uint64_t i = 0; i < n; ++i) {
    random::RngStream rng(cfg_.seed, i, 0);
    path[0] = model.spot;
    double S = model.spot;
    for (int t = 0; t < cfg_.n_steps; ++t) {
      double z = rng.normal01();
      S = process.step(S, dt, z);
      path[t + 1] = S;
    }
    double pay = payoff(path);
    if (anti) {
      random::RngStream rng_a(cfg_.seed, i, 0);
      path[0] = model.spot;
      S = model.spot;
      for (int t = 0; t < cfg_.n_steps; ++t) {
        double z = -rng_a.normal01();
        S = process.step(S, dt, z);
        path[t + 1] = S;
      }
      pay = 0.5 * (pay + payoff(path));
    }
    acc.add(disc * pay);
  }

  McResult r;
  r.mean = acc.mean();
  r.variance = acc.variance();
  r.std_dev = acc.std_dev();
  r.std_error = acc.std_error();
  r.n_paths = n;
  const double zcrit = 1.959963984540054;
  r.ci_low = r.mean - zcrit * r.std_error;
  r.ci_high = r.mean + zcrit * r.std_error;
  r.runtime_sec = timer.elapsed();
  r.backend = "cpu";
  return r;
}

McResult MonteCarloEngine::price_gbm_european_call(const stochastic::GbmParams& model, double strike) const {
  auto payoff = [strike](const std::vector<double>& path) {
    return std::max(path.back() - strike, 0.0);
  };
  return price_gbm(model, payoff);
}

}  // namespace ammc::mc
