# Adaptive Mesh Refinement Monte-Carlo Engine for Path-Dependent Derivatives with Material Nonlinearity

📢 **Release Notice:** This repository contains the complete codebase for this project, engineered between *March 2, 2026* and *October 1, 2026*. The work was developed intermittently alongside other research software and has been packaged in full for public viewing and use. Relative to my other projects this one is more ambitious in scope — adaptive mesh refinement coupled to Monte Carlo pricing of path-dependent exotics, with constitutive-style volatility evolution, goal-oriented residuals, and an explicit error budget — but the honest reason it exists is practical: I built it to deepen my own understanding of path-dependent pricing, numerical discretization error, and volatility dynamics while day trading, not to ship a trading product or claim an edge.

**Author:** Nabil Khondaker  
**Version:** 0.1.0  
**Language:** C++20  

## Overview

This repository implements a research computational engine that combines Monte Carlo methods for pricing path-dependent exotic derivatives with adaptive spatial and temporal discretization techniques inspired by computational mechanics and finite-element residual error estimation. Volatility dynamics are evolved through a constitutive modeling framework that borrows mathematical structures from nonlinear material mechanics (memory kernels, internal state variables, relaxation, and damage-style evolution) while remaining strictly a mathematical analogy, not a physical claim about markets.

The system is designed as a coherent numerical architecture rather than a collection of pricing formulas. It supports multi-asset stochastic processes (including stochastic volatility and jumps), generic path-dependent payoffs, goal-oriented adaptive refinement, automatic differentiation for Greeks, and deterministic reproducibility under a declared execution profile.

## Motivation

Path-dependent derivatives (Asian, barrier, lookback, occupation-time, variance-linked, multi-asset barrier baskets, etc.) are sensitive to the quality of the numerical discretization of both the underlying stochastic dynamics and any auxiliary state fields (for example, a volatility surface or constitutive memory variables). Fixed uniform meshes or fixed timesteps can waste computational effort in regions that contribute little to the target quantity (price or a Greek) while under-resolving critical regions near barriers, exercise boundaries, or high-curvature regions of the volatility field.

Adaptive refinement driven by local residual indicators and goal-oriented estimators aims to allocate degrees of freedom where they most affect the quantity of interest. Coupling this idea with a constitutive representation of evolving volatility provides a flexible research platform for studying memory effects, hysteresis-like responses, and path-dependent risk measures.

## Research Questions

The repository is structured to investigate (not to presuppose answers to) questions such as:

- Does goal-oriented adaptive refinement reduce work for a fixed pricing tolerance relative to uniform refinement?
- Does refinement targeted at price accuracy also improve Greek accuracy?
- How does constitutive memory alter the distribution of path-dependent payoffs?
- How do jumps interact with adaptive temporal refinement?
- How does barrier proximity influence local residual indicators?
- How do Monte Carlo statistical error and spatial/temporal discretization error compare under a fixed budget?
- What variance reduction is obtained from control variates and antithetic sampling on representative exotics?
- How do pathwise automatic-differentiation Greeks compare with finite-difference and likelihood-ratio estimators?
- What reproducibility guarantees survive OpenMP, MPI, and CUDA parallelization under a strict deterministic mode?

## What This Project Does

- Prices a range of path-dependent and multi-asset exotic derivatives via Monte Carlo.
- Supports GBM, local volatility, Heston-style stochastic volatility, and jump-diffusion (including combined SV+jump) dynamics.
- Provides a generic payoff interface and concrete products (European, Asian, barrier, lookback, digital, basket, worst-of/best-of, variance-linked).
- Evolves volatility through a constitutive model with internal variables, relaxation, memory kernels, and a damage-style state variable (mathematical analogy only).
- Constructs and adapts a mesh over a discretized state space; estimates local residuals; supports residual-driven and simplified goal-oriented refinement.
- Adapts timesteps according to local error indicators, barrier proximity, and constitutive state change.
- Computes pathwise Greeks via dual-number automatic differentiation and compares them with finite differences.
- Implements antithetic variates, control variates, and online statistics with confidence intervals.
- Supports OpenMP parallelism; optional MPI and CUDA backends when available.
- Records full reproducibility manifests (seeds, path IDs, compiler, architecture, deterministic mode).
- Provides unit tests, analytical validation cases (Black-Scholes, geometric Asian, selected barriers), and an experiment suite.

## What It Does Not Do

- It does not claim that financial volatility is a viscoelastic material. The constitutive framework is a mathematical modeling device.
- It does not guarantee that adaptive refinement always improves every error metric; experiments may report negative results.
- It does not provide production-grade trading-system guarantees, live-market calibration, or investment advice.
- It does not claim bitwise reproducibility across arbitrary CPU/GPU architectures or compiler versions outside a declared execution profile.
- It does not implement a full dual-weighted residual (DWR) adjoint solver; a simplified goal-oriented residual indicator is provided and documented as such.
- It does not replace independent model validation, market-risk frameworks, or regulatory model review.

## Mathematical Formulation (Summary)

### Stochastic Dynamics

Geometric Brownian motion:

\[
dS_t = \mu S_t\,dt + \sigma S_t\,dW_t
\]

Heston-style stochastic volatility (with correlated Brownian motions):

\[
\begin{aligned}
dS_t &= \mu S_t\,dt + \sqrt{v_t}\,S_t\,dW_t^1,\\
dv_t &= \kappa(\theta - v_t)\,dt + \xi\sqrt{v_t}\,dW_t^2,\\
\langle dW^1,dW^2\rangle &= \rho\,dt.
\end{aligned}
\]

Jump-diffusion (Merton-style lognormal jumps) is supported with intensity \(\lambda\) and jump-size distribution; the compensator is applied where required for the drift.

Multi-asset dynamics use a correlation matrix \(\Sigma = LL^\top\) obtained via Cholesky factorization (with diagnostics for positive-definiteness and near-singularity).

### Constitutive Evolution (Mathematical Analogy)

Internal variables \(q_i\) obey relaxation laws of the form

\[
\frac{dq_i}{dt} = \frac{f_i(\text{state}) - q_i}{\tau_i}.
\]

A damage-style variable \(D\in[0,1]\) evolves according to a configurable accumulation law. Volatility is reconstructed from the instantaneous state, the memory variables, and \(D\). This is a constitutive modeling device, not a physical equivalence claim.

### Adaptive Residual Indicators

Local indicators of the form \(\eta_K \approx \|R_K\|\) are computed on mesh cells. A simplified goal-oriented weight focuses refinement on contributions to a target functional (price, delta, or vega). Full dual-weighted residual theory is left as future work; the present estimators are documented as residual-based indicators, not rigorous a-posteriori bounds.

### Monte Carlo and Error Decomposition

The reported total numerical uncertainty is conceptually decomposed into Monte Carlo statistical error, temporal discretization error, spatial discretization error, and model error (the last not measurable from simulation alone). Standard error, confidence intervals, and bias diagnostics are reported separately.

## Computational Architecture

```
Market / model configuration
        ↓
Stochastic factors + correlation structure
        ↓
Volatility surface + constitutive state
        ↓
Initial mesh + time discretization
        ↓
Path generation (deterministic stream IDs)
        ↓
Local residual / goal-oriented estimation
        ↓
Adaptive spatial / temporal refinement
        ↓
Pathwise automatic differentiation
        ↓
Payoff evaluation + Greek accumulation
        ↓
Monte Carlo aggregation + statistics
        ↓
Error budget + reproducibility manifest
        ↓
Report (JSON / Markdown / CSV)
```

## High-Performance Computing

- **OpenMP**: path-level and residual evaluation parallelism; deterministic scheduling available in strict mode.
- **MPI**: optional distributed path partitioning by global path ID; deterministic aggregation when enabled.
- **CUDA**: optional kernels for path generation and payoff evaluation when CUDA is available at build time.
- Hybrid configurations (MPI × OpenMP × CUDA) are supported when the corresponding libraries are detected.

## Reproducibility

A **Strict Deterministic Mode** records global seed, rank, thread, path-range, RNG algorithm, compiler, architecture, and reduction ordering. Bitwise reproducibility is guaranteed only within a declared execution profile (compiler version, architecture, CUDA version, MPI implementation, build flags). A numerical-equivalence mode is provided for cross-platform comparison within documented tolerances.

## Automatic Differentiation and Greeks

Forward-mode dual numbers propagate derivatives through the stochastic steps, constitutive updates, and smooth payoffs. Pathwise differentiation is used where valid; limitations near discontinuous payoffs (barriers, digitals) are documented. Likelihood-ratio / score estimators are available for discontinuous cases. Finite-difference benchmarks are provided for verification.

## Installation and Build

### Dependencies

- C++20 compiler (GCC ≥ 10, Clang ≥ 12, or MSVC 2019+)
- CMake ≥ 3.18
- Optional: OpenMP, MPI, CUDA Toolkit

### Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DAMMC_ENABLE_OPENMP=ON
cmake --build . -j
ctest --output-on-failure
```

Optional flags:

```bash
-DAMMC_ENABLE_MPI=ON
-DAMMC_ENABLE_CUDA=ON
-DAMMC_STRICT_DETERMINISTIC=ON
```

## CLI Examples

```bash
# Black-Scholes European validation
./ammc_price --config configs/experiments/01_black_scholes_validation.json

# Multi-asset barrier Asian with constitutive memory
./ammc_price --config configs/experiments/hero_multi_asset_barrier_asian.json

# Greeks via AD
./ammc_greeks --config configs/greeks/heston_asian_ad.json

# Deterministic replay
./ammc_reproduce --manifest results/reproducibility_manifest.json
```

## Testing and Validation

Unit tests cover RNG streams, Cholesky, stochastic integrators, constitutive updates, mesh hierarchy, residual estimators, dual-number AD, and online statistics. Analytical validations include Black-Scholes Europeans, geometric Asians, and selected barrier formulas. Stochastic-volatility and jump cases are compared against independent numerical references generated inside the repository.

## Experiments

The `experiments/` tree contains configuration-driven studies corresponding to the numbered list in the project specification (validation, adaptive mesh, goal-oriented refinement, constitutive memory, variance reduction, AD vs FD, scaling, reproducibility, synthetic calibration, error budget, etc.). Each experiment directory includes a README describing expected behavior, the command line, and the result schema.

## Limitations (Explicit)

- This is a research and computational software project.
- Stochastic models are mathematical assumptions, not market truths.
- Constitutive analogies are modeling devices.
- Adaptive estimators are model- and discretization-specific.
- Numerical convergence does not imply model correctness.
- Monte Carlo precision does not remove model risk.
- Synthetic calibration is not live-market calibration.
- No investment returns are guaranteed.
- This is not financial advice.
- This is not a production trading system unless independently hardened and validated.
- Results depend on parameters, discretization, seeds, implementation, hardware, and modeling assumptions.

## Research Directions

Potential extensions (not implemented): full dual-weighted residual adjoint methods, multilevel Monte Carlo / MLQMC, sparse grids, discontinuous Galerkin formulations, rough-volatility models, fractional memory kernels, multi-GPU clusters, fault-tolerant Monte Carlo, and formal model-risk frameworks.

## Citation

See `CITATION.cff` and `AUTHORS.md`.

## License

See `LICENSE`.

## Author

Primary author: **Nabil Khondaker**

---

Numerical results are generated by the reproducible experiment suite. No benchmark or pricing result is claimed without an executed run.
