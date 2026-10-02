#include "ammc/monte_carlo/engine.hpp"
#include "ammc/stochastic/gbm.hpp"
#include "ammc/core/version.hpp"
#include <iostream>
#include <cmath>
#include <iomanip>

// Analytical Black-Scholes call for validation
static double norm_cdf(double x) {
  return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

static double bs_call(double S, double K, double r, double q, double sigma, double T) {
  if (T <= 0.0) return std::max(S - K, 0.0);
  double vol_sqrt_t = sigma * std::sqrt(T);
  double d1 = (std::log(S / K) + (r - q + 0.5 * sigma * sigma) * T) / vol_sqrt_t;
  double d2 = d1 - vol_sqrt_t;
  return S * std::exp(-q * T) * norm_cdf(d1) - K * std::exp(-r * T) * norm_cdf(d2);
}

int main(int argc, char** argv) {
  std::cout << ammc::kProjectName << " v" << ammc::kVersion
            << "  (author: " << ammc::kAuthor << ")\n\n";

  ammc::stochastic::GbmParams model;
  model.spot = 100.0;
  model.rate = 0.05;
  model.dividend = 0.0;
  model.vol = 0.20;
  model.maturity = 1.0;
  double strike = 100.0;

  ammc::mc::McConfig cfg;
  cfg.n_paths = 200000;
  cfg.seed = 42;
  cfg.n_steps = 252;
  cfg.antithetic = true;

  if (argc > 1) {
    // simple arg parsing could be added
  }

  ammc::mc::MonteCarloEngine engine(cfg);
  auto result = engine.price_gbm_european_call(model, strike);

  double analytic = bs_call(model.spot, strike, model.rate, model.dividend, model.vol, model.maturity);

  std::cout << std::fixed << std::setprecision(6);
  std::cout << "European Call (GBM / Black-Scholes validation)\n";
  std::cout << "----------------------------------------------\n";
  std::cout << "Spot / Strike / r / q / vol / T : "
            << model.spot << " / " << strike << " / " << model.rate << " / "
            << model.dividend << " / " << model.vol << " / " << model.maturity << "\n";
  std::cout << "Paths / Steps / Seed / Antithetic : "
            << result.n_paths << " / " << cfg.n_steps << " / " << cfg.seed
            << " / " << (cfg.antithetic ? "yes" : "no") << "\n\n";
  std::cout << "MC price          : " << result.mean << "\n";
  std::cout << "Std. error        : " << result.std_error << "\n";
  std::cout << "95% CI            : [" << result.ci_low << ", " << result.ci_high << "]\n";
  std::cout << "Analytical BS     : " << analytic << "\n";
  std::cout << "Abs. error        : " << std::abs(result.mean - analytic) << "\n";
  std::cout << "Runtime (s)       : " << result.runtime_sec << "\n";
  std::cout << "Backend           : " << result.backend << "\n";

  return 0;
}
