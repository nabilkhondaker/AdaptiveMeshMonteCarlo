#pragma once
#include "ammc/random/rng.hpp"
#include "ammc/automatic_differentiation/dual.hpp"
#include <vector>

namespace ammc::stochastic {

struct GbmParams {
  double spot{100.0};
  double rate{0.05};
  double dividend{0.0};
  double vol{0.20};
  double maturity{1.0};
};

/// Euler–Maruyama step for geometric Brownian motion.
/// Supports dual numbers for pathwise differentiation w.r.t. spot or vol.
class GbmProcess {
 public:
  explicit GbmProcess(GbmParams p) : params_(p) {}

  double step(double S, double dt, double z) const {
    const double drift = (params_.rate - params_.dividend - 0.5 * params_.vol * params_.vol) * dt;
    const double diffusion = params_.vol * std::sqrt(dt) * z;
    return S * std::exp(drift + diffusion);
  }

  ad::Dual<double> step_dual(ad::Dual<double> S, double dt, double z) const {
    using ad::Dual;
    Dual<double> vol(params_.vol, 0.0);  // can be made dual for vega
    Dual<double> drift = (params_.rate - params_.dividend - 0.5 * vol * vol) * dt;
    Dual<double> diffusion = vol * std::sqrt(dt) * z;
    return S * exp(drift + diffusion);
  }

  /// Pathwise delta: dS_T / dS_0 under GBM is S_T / S_0.
  static double pathwise_delta_factor(double S_T, double S_0) {
    return S_0 > 0.0 ? S_T / S_0 : 0.0;
  }

  const GbmParams& params() const { return params_; }

 private:
  GbmParams params_;
};

}  // namespace ammc::stochastic
