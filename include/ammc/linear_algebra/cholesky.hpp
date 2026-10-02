#pragma once
#include <vector>
#include <stdexcept>
#include <cmath>

namespace ammc::linalg {

/// Dense Cholesky factorization for correlation / covariance matrices.
/// Returns lower-triangular L such that L L^T = A.
/// Throws if matrix is not positive definite (with diagnostics).
class Cholesky {
 public:
  explicit Cholesky(std::size_t n);

  /// Factorize symmetric matrix A (row-major, n x n). Returns true on success.
  bool factorize(const std::vector<double>& A);

  /// Apply L to a vector: y = L z
  void apply(const std::vector<double>& z, std::vector<double>& y) const;

  /// Reconstruct A ≈ L L^T for validation
  void reconstruct(std::vector<double>& A_approx) const;

  bool valid() const { return valid_; }
  double min_pivot() const { return min_pivot_; }
  const std::vector<double>& L() const { return L_; }

 private:
  std::size_t n_;
  std::vector<double> L_;
  bool valid_{false};
  double min_pivot_{0.0};
};

/// Validate correlation matrix: symmetric, unit diagonal, positive definite.
struct CorrelationDiagnostics {
  bool is_symmetric{false};
  bool unit_diagonal{false};
  bool positive_definite{false};
  double max_asymmetry{0.0};
  double min_eigenvalue_proxy{0.0};
  std::string message;
};

CorrelationDiagnostics diagnose_correlation(const std::vector<double>& C, std::size_t n);

}  // namespace ammc::linalg
