# Research Documentation — Adaptive Mesh Monte-Carlo Engine

**Author:** Nabil Khondaker

## 1. Stochastic Calculus Formulation

The baseline asset dynamics under the risk-neutral measure are

\[
dS_t = (r-q)S_t\,dt + \sigma_t S_t\,dW_t
\]

where \(\sigma_t\) may be deterministic (Black–Scholes), a function of state and time (local volatility), or itself a stochastic process (Heston). In the Heston case

\[
dv_t = \kappa(\theta-v_t)\,dt + \xi\sqrt{v_t}\,dW_t^v,\qquad
\langle dW,dW^v\rangle=\rho\,dt.
\]

Jumps are introduced via a compensated Poisson random measure. Multi-asset dynamics replace the scalar Brownian motion by a correlated vector obtained from the Cholesky factor of the instantaneous correlation matrix.

## 2. Constitutive Modeling of Volatility (Mathematical Analogy)

We introduce internal variables \(q_i(t)\) obeying linear (or nonlinear) relaxation

\[
\dot q_i = \frac{f_i(S,v,t)-q_i}{\tau_i}.
\]

A damage-style state \(D\in[0,1]\) accumulates according to a non-negative rate function of instantaneous “stress” proxies (volatility, realized variance, barrier proximity). The instantaneous volatility used in the SDE is a smooth function of the current state and the internal variables. This construction is a constitutive analogy; it does not assert physical viscoelasticity of markets.

## 3. Adaptive Spatial Mesh and Residual Indicators

A mesh is constructed over a discretized state domain (log-spot, variance factor, etc.). On each cell \(K\) a residual indicator \(\eta_K\) is evaluated. A simplified goal-oriented weight multiplies the residual by a proxy of the dual solution sensitivity with respect to a target functional (price or a Greek). Refinement is triggered when \(\eta_K>\theta\max\eta\).

## 4. Temporal Adaptivity

Local truncation error, barrier proximity, and constitutive stiffness drive a PI controller for the time step subject to user-specified \(\Delta t_{\min}\) and \(\Delta t_{\max}\).

## 5. Monte Carlo Error and Bias Decomposition

Reported uncertainty is separated into

- Monte Carlo statistical error (standard error of the mean),
- temporal discretization bias (estimated by step-size refinement),
- spatial discretization bias (estimated by residual indicators),
- model error (not measurable from simulation alone).

## 6. Automatic Differentiation

Forward-mode dual numbers propagate first derivatives through smooth maps (SDE steps, constitutive updates, continuous payoffs). Pathwise differentiation is valid under standard conditions; discontinuous payoffs require likelihood-ratio or smoothed approximations. Finite-difference verification suites are included.

## 7. Reproducibility

Strict deterministic mode fixes global seed, path IDs, reduction order, and OpenMP schedule. Bitwise identity is guaranteed only inside a declared execution profile. Cross-platform numerical equivalence is verified within documented tolerances.

## 8. Limitations

Adaptive estimators are model- and discretization-specific. Constitutive parameters are research parameters, not calibrated market truths. Synthetic calibration experiments recover known parameters under ideal conditions; they do not constitute live-market validation.

## 9. Research Questions

See the main README. Experiments are designed to produce both positive and negative evidence.

