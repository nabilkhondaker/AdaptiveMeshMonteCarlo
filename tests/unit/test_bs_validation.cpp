#include "ammc/monte_carlo/engine.hpp"
#include "ammc/stochastic/gbm.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

static double norm_cdf(double x) {
  return 0.5 * std::erfc(-x / std::sqrt(2.0));
}
static double bs_call(double S, double K, double r, double q, double sigma, double T) {
  double vst = sigma * std::sqrt(T);
  double d1 = (std::log(S/K) + (r-q+0.5*sigma*sigma)*T) / vst;
  double d2 = d1 - vst;
  return S*std::exp(-q*T)*norm_cdf(d1) - K*std::exp(-r*T)*norm_cdf(d2);
}

int test_bs_validation() {
  ammc::stochastic::GbmParams m{100, 0.05, 0.0, 0.2, 1.0};
  ammc::mc::McConfig cfg;
  cfg.n_paths = 50000;
  cfg.seed = 7;
  cfg.n_steps = 100;
  cfg.antithetic = true;
  ammc::mc::MonteCarloEngine eng(cfg);
  auto r = eng.price_gbm_european_call(m, 100.0);
  double ana = bs_call(100,100,0.05,0,0.2,1.0);
  double err = std::abs(r.mean - ana);
  std::cout << "MC=" << r.mean << " BS=" << ana << " err=" << err << " se=" << r.std_error << "\n";
  assert(err < 0.15);  // loose tolerance for 50k paths
  std::cout << "test_bs_validation PASSED\n";
  return 0;
}
