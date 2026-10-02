#pragma once
#include <cmath>

namespace ammc::math {

constexpr double kPi = 3.14159265358979323846;
constexpr double kSqrt2Pi = 2.50662827463100050241;
constexpr double kInvSqrt2Pi = 0.39894228040143267794;
constexpr double kTiny = 1.0e-16;
constexpr double kHuge = 1.0e16;

inline double clamp_positive(double x, double floor = kTiny) {
  return x < floor ? floor : x;
}

}  // namespace ammc::math
