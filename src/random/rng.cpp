#include "ammc/random/rng.hpp"
#include "ammc/math/constants.hpp"
#include <cmath>
#include <stdexcept>

namespace ammc::random {

namespace {

std::uint64_t splitmix64(std::uint64_t& x) {
  x += 0x9e3779b97f4a7c15ULL;
  std::uint64_t z = x;
  z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
  z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
  return z ^ (z >> 31);
}

// Acklam inverse normal CDF approximation
double inv_norm_cdf(double p) {
  if (p <= 0.0) return -1.0e10;
  if (p >= 1.0) return 1.0e10;
  static const double a1 = -3.969683028665376e+01;
  static const double a2 = 2.209460984245205e+02;
  static const double a3 = -2.759285104469687e+02;
  static const double a4 = 1.383577459334128e+02;
  static const double a5 = -3.066479806614716e+01;
  static const double a6 = 2.506628277459239e+00;
  static const double b1 = -5.447609879822406e+01;
  static const double b2 = 1.615858368580409e+02;
  static const double b3 = -1.556989798598866e+02;
  static const double b4 = 6.680131188771972e+01;
  static const double b5 = -1.328068155288572e+01;
  static const double c1 = -7.784894002430293e-03;
  static const double c2 = -3.223964580411365e-01;
  static const double c3 = -2.400758277161838e+00;
  static const double c4 = -2.549732539343734e+00;
  static const double c5 = 4.374664141464968e+00;
  static const double c6 = 2.938163982698783e+00;
  static const double d1 = 7.784695709041462e-03;
  static const double d2 = 3.224671290700398e-01;
  static const double d3 = 2.445134137142996e+00;
  static const double d4 = 3.754408661907416e+00;
  const double p_low = 0.02425;
  const double p_high = 1.0 - p_low;
  double q, r;
  if (p < p_low) {
    q = std::sqrt(-2.0 * std::log(p));
    return (((((c1 * q + c2) * q + c3) * q + c4) * q + c5) * q + c6) /
           ((((d1 * q + d2) * q + d3) * q + d4) * q + 1.0);
  }
  if (p <= p_high) {
    q = p - 0.5;
    r = q * q;
    return (((((a1 * r + a2) * r + a3) * r + a4) * r + a5) * r + a6) * q /
           (((((b1 * r + b2) * r + b3) * r + b4) * r + b5) * r + 1.0);
  }
  q = std::sqrt(-2.0 * std::log(1.0 - p));
  return -(((((c1 * q + c2) * q + c3) * q + c4) * q + c5) * q + c6) /
         ((((d1 * q + d2) * q + d3) * q + d4) * q + 1.0);
}

}  // namespace

RngStream::RngStream(std::uint64_t global_seed, std::uint64_t path_id, std::uint64_t substream) {
  seed(global_seed, path_id, substream);
}

void RngStream::seed(std::uint64_t global_seed, std::uint64_t path_id, std::uint64_t substream) {
  global_seed_ = global_seed;
  path_id_ = path_id;
  substream_ = substream;
  counter_ = 0;
  // Mix seeds into initial state
  state_ = global_seed;
  state_ = splitmix64(state_);
  state_ ^= path_id * 0x9e3779b97f4a7c15ULL;
  state_ = splitmix64(state_);
  state_ ^= substream * 0xbf58476d1ce4e5b9ULL;
  state_ = splitmix64(state_);
}

std::uint64_t RngStream::next_u64() {
  ++counter_;
  return splitmix64(state_);
}

double RngStream::uniform01() {
  // 53-bit mantissa
  const std::uint64_t u = next_u64();
  return (u >> 11) * (1.0 / 9007199254740992.0);
}

double RngStream::normal01() {
  return inv_norm_cdf(uniform01());
}

void RngStream::normal_pair(double& z1, double& z2) {
  // Box-Muller
  double u1 = uniform01();
  double u2 = uniform01();
  if (u1 < math::kTiny) u1 = math::kTiny;
  const double r = std::sqrt(-2.0 * std::log(u1));
  const double theta = 2.0 * math::kPi * u2;
  z1 = r * std::cos(theta);
  z2 = r * std::sin(theta);
}

}  // namespace ammc::random
