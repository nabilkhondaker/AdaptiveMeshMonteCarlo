#include "ammc/linear_algebra/cholesky.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>

namespace ammc::linalg {

Cholesky::Cholesky(std::size_t n) : n_(n), L_(n * n, 0.0) {}

bool Cholesky::factorize(const std::vector<double>& A) {
  if (A.size() != n_ * n_) {
    valid_ = false;
    return false;
  }
  L_.assign(n_ * n_, 0.0);
  min_pivot_ = 1.0e300;
  for (std::size_t i = 0; i < n_; ++i) {
    for (std::size_t j = 0; j <= i; ++j) {
      double sum = A[i * n_ + j];
      for (std::size_t k = 0; k < j; ++k) {
        sum -= L_[i * n_ + k] * L_[j * n_ + k];
      }
      if (i == j) {
        if (sum <= 0.0) {
          valid_ = false;
          min_pivot_ = sum;
          return false;
        }
        L_[i * n_ + j] = std::sqrt(sum);
        min_pivot_ = std::min(min_pivot_, L_[i * n_ + j]);
      } else {
        if (std::abs(L_[j * n_ + j]) < 1.0e-18) {
          valid_ = false;
          return false;
        }
        L_[i * n_ + j] = sum / L_[j * n_ + j];
      }
    }
  }
  valid_ = true;
  return true;
}

void Cholesky::apply(const std::vector<double>& z, std::vector<double>& y) const {
  if (!valid_ || z.size() != n_) {
    throw std::runtime_error("Cholesky::apply: invalid state or dimension");
  }
  y.resize(n_);
  for (std::size_t i = 0; i < n_; ++i) {
    double s = 0.0;
    for (std::size_t j = 0; j <= i; ++j) {
      s += L_[i * n_ + j] * z[j];
    }
    y[i] = s;
  }
}

void Cholesky::reconstruct(std::vector<double>& A_approx) const {
  A_approx.assign(n_ * n_, 0.0);
  for (std::size_t i = 0; i < n_; ++i) {
    for (std::size_t j = 0; j < n_; ++j) {
      double s = 0.0;
      for (std::size_t k = 0; k <= std::min(i, j); ++k) {
        s += L_[i * n_ + k] * L_[j * n_ + k];
      }
      A_approx[i * n_ + j] = s;
    }
  }
}

CorrelationDiagnostics diagnose_correlation(const std::vector<double>& C, std::size_t n) {
  CorrelationDiagnostics d;
  if (C.size() != n * n) {
    d.message = "size mismatch";
    return d;
  }
  double max_asym = 0.0;
  bool unit_diag = true;
  for (std::size_t i = 0; i < n; ++i) {
    if (std::abs(C[i * n + i] - 1.0) > 1.0e-10) unit_diag = false;
    for (std::size_t j = i + 1; j < n; ++j) {
      max_asym = std::max(max_asym, std::abs(C[i * n + j] - C[j * n + i]));
    }
  }
  d.is_symmetric = max_asym < 1.0e-10;
  d.unit_diagonal = unit_diag;
  d.max_asymmetry = max_asym;

  Cholesky chol(n);
  d.positive_definite = chol.factorize(C);
  d.min_eigenvalue_proxy = chol.min_pivot();
  if (!d.is_symmetric) d.message = "not symmetric";
  else if (!d.unit_diagonal) d.message = "diagonal not unit";
  else if (!d.positive_definite) d.message = "not positive definite";
  else d.message = "ok";
  return d;
}

}  // namespace ammc::linalg
