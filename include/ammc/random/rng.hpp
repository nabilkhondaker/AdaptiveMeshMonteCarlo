#pragma once
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace ammc::random {

/// Counter-based deterministic RNG (SplitMix64 + Philox-inspired mixing).
/// Every path is identified by a global path ID; the stream is derived
/// deterministically from (global_seed, path_id, substream).
class RngStream {
 public:
  RngStream() = default;
  RngStream(std::uint64_t global_seed, std::uint64_t path_id, std::uint64_t substream = 0);

  void seed(std::uint64_t global_seed, std::uint64_t path_id, std::uint64_t substream = 0);

  /// Uniform [0,1)
  double uniform01();

  /// Standard normal via inverse CDF (Acklam approximation)
  double normal01();

  /// Box-Muller pair (for efficiency when many normals needed)
  void normal_pair(double& z1, double& z2);

  std::uint64_t global_seed() const { return global_seed_; }
  std::uint64_t path_id() const { return path_id_; }
  std::uint64_t substream() const { return substream_; }
  std::uint64_t counter() const { return counter_; }

  static std::string algorithm_name() { return "SplitMix64+Acklam"; }

 private:
  std::uint64_t state_{0};
  std::uint64_t global_seed_{0};
  std::uint64_t path_id_{0};
  std::uint64_t substream_{0};
  std::uint64_t counter_{0};

  std::uint64_t next_u64();
};

/// Hierarchy of seeds for MPI ranks / OpenMP threads / path batches.
struct SeedHierarchy {
  std::uint64_t global_seed{0};
  int rank{0};
  int thread{0};
  std::uint64_t path_begin{0};
  std::uint64_t path_end{0};
  std::string algorithm{"SplitMix64+Acklam"};
};

}  // namespace ammc::random
