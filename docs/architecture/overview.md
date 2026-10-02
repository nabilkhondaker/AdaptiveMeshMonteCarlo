# Architecture Overview

Author: Nabil Khondaker

The engine is organized into layers:

1. Random number streams (deterministic, path-ID based)
2. Stochastic process integrators (GBM, Heston, jumps)
3. Constitutive state evolution (memory, relaxation, damage-style)
4. Mesh hierarchy and residual estimators
5. Path generation and payoff evaluation
6. Automatic differentiation (dual numbers)
7. Monte Carlo aggregation and statistics
8. Reporting and reproducibility manifests

Parallelism is introduced at the path level (OpenMP) and optionally across ranks (MPI) and devices (CUDA).
