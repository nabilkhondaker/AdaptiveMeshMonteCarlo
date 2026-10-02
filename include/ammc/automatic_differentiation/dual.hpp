#pragma once
#include <cmath>
#include <type_traits>

namespace ammc::ad {

/// Forward-mode dual number for pathwise automatic differentiation.
/// Value + first derivative with respect to a single independent variable
/// (or a directional derivative). For multi-parameter Greeks, multiple
/// dual passes or a vector dual can be used.
template <typename T = double>
struct Dual {
  T value{0};
  T deriv{0};

  Dual() = default;
  Dual(T v) : value(v), deriv(0) {}
  Dual(T v, T d) : value(v), deriv(d) {}

  Dual& operator+=(const Dual& o) {
    value += o.value;
    deriv += o.deriv;
    return *this;
  }
  Dual& operator-=(const Dual& o) {
    value -= o.value;
    deriv -= o.deriv;
    return *this;
  }
  Dual& operator*=(const Dual& o) {
    deriv = deriv * o.value + value * o.deriv;
    value *= o.value;
    return *this;
  }
  Dual& operator/=(const Dual& o) {
    deriv = (deriv * o.value - value * o.deriv) / (o.value * o.value);
    value /= o.value;
    return *this;
  }
};

template <typename T>
Dual<T> operator+(Dual<T> a, const Dual<T>& b) { return a += b; }
template <typename T>
Dual<T> operator-(Dual<T> a, const Dual<T>& b) { return a -= b; }
template <typename T>
Dual<T> operator*(Dual<T> a, const Dual<T>& b) { return a *= b; }
template <typename T>
Dual<T> operator/(Dual<T> a, const Dual<T>& b) { return a /= b; }

template <typename T>
Dual<T> operator+(Dual<T> a, T s) { a.value += s; return a; }
template <typename T>
Dual<T> operator+(T s, Dual<T> a) { a.value += s; return a; }
template <typename T>
Dual<T> operator-(Dual<T> a, T s) { a.value -= s; return a; }
template <typename T>
Dual<T> operator-(T s, Dual<T> a) { return Dual<T>(s - a.value, -a.deriv); }
template <typename T>
Dual<T> operator*(Dual<T> a, T s) { a.value *= s; a.deriv *= s; return a; }
template <typename T>
Dual<T> operator*(T s, Dual<T> a) { a.value *= s; a.deriv *= s; return a; }
template <typename T>
Dual<T> operator/(Dual<T> a, T s) { a.value /= s; a.deriv /= s; return a; }
template <typename T>
Dual<T> operator/(T s, Dual<T> a) {
  return Dual<T>(s / a.value, -s * a.deriv / (a.value * a.value));
}

template <typename T>
Dual<T> exp(const Dual<T>& x) {
  T e = std::exp(x.value);
  return Dual<T>(e, e * x.deriv);
}
template <typename T>
Dual<T> log(const Dual<T>& x) {
  return Dual<T>(std::log(x.value), x.deriv / x.value);
}
template <typename T>
Dual<T> sqrt(const Dual<T>& x) {
  T s = std::sqrt(x.value);
  return Dual<T>(s, x.deriv / (2 * s));
}
template <typename T>
Dual<T> abs(const Dual<T>& x) {
  return x.value >= 0 ? x : Dual<T>(-x.value, -x.deriv);
}
template <typename T>
Dual<T> max(const Dual<T>& a, T b) {
  return a.value >= b ? a : Dual<T>(b, 0);
}
template <typename T>
Dual<T> max(T a, const Dual<T>& b) {
  return a >= b.value ? Dual<T>(a, 0) : b;
}

}  // namespace ammc::ad
