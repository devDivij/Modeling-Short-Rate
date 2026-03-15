# Short-Rate Models Implementation

C++ implementations of classical short-rate models for interest rate modeling and bond pricing.

## Models Implemented

### 1. **Vasicek Model (Exact Discretization)**

The Vasicek model describes the evolution of the short-term interest rate as a mean-reverting process.

**Stochastic Differential Equation (SDE):**


$$dr(t) = \alpha(b - r(t))dt + \sigma dW(t)$$

* `alpha` ($\alpha$): Speed of mean reversion.
* `b` ($b$): Long-term mean level (the target rate).

**Exact Transition Formula:**
The code implements the analytical solution for a time step $\Delta t$, ensuring the simulated rates follow the exact theoretical distribution:


$$r(t + \Delta t) = r(t)e^{-\alpha \Delta t} + b(1 - e^{-\alpha \Delta t}) + \sigma \sqrt{\frac{1 - e^{-2\alpha \Delta t}}{2\alpha}} \cdot Z$$


where $Z \sim N(0, 1)$.

---


### 2. **Cox-Ingersoll-Ross (CIR) Square Root Diffusion Model**

The CIR model is an extension of the Vasicek model that ensures interest rates stay non-negative by making volatility dependent on the square root of the current rate.

**Stochastic Differential Equation (SDE):**


$$dr(t) = \alpha(b - r(t))dt + \sigma \sqrt{r(t)} dW(t)$$

**Numerical Scheme (Euler-Maruyama):**
The code implements the Euler discretization, which approximates the change over a small time step $\Delta t$:


$$r_{t+\Delta t} = r_t + \alpha(b - r_t)\Delta t + \sigma \sqrt{\max(r_t, 0)} \sqrt{\Delta t} \cdot Z$$


where $Z \sim N(0, 1)$.

---

### 3. **Square Root Diffusion Model through Transition Density**

This implementation utilizes the fact that in a CIR model, the future rate $r(t+\Delta t)$ follows a **Non-Central Chi-Squared Distribution**.

**Stochastic Differential Equation (SDE):**


$$dr(t) = \alpha(b - r(t))dt + \sigma \sqrt{r(t)} dW(t)$$

**Exact Transition Formula:**
The code calculates the rate at the next time step by sampling from the distribution of $r(t+\Delta t)$ given $r(t)$. The scaling factor `c` is defined as:


$$c = \frac{\sigma^2(1 - e^{-\alpha \Delta t})}{4\alpha}$$

**Implementation Logic based on Degrees of Freedom ($d$):**

`d`: Degrees of freedom, calculated as $d = \frac{4b\alpha}{\sigma^2}$.
The code uses two different mathematical identities to sample the non-central chi-squared distribution:

1. **Case $d > 1$ (High Mean Reversion/Low Vol):**
    The rate is sampled using the property that a non-central chi-square variable can be represented as the sum of a squared normal distribution (with a shift) and a central chi-square distribution:

    $$r_{t+\Delta t} = c \cdot \left( (Z + \sqrt{\lambda})^2 + \chi^2_{d-1} \right)$$
    where $\lambda = \frac{r_t e^{-\alpha \Delta t}}{c}$ is the non-centrality parameter.
2. **Case $d \le 1$ (Low Mean Reversion/High Vol):**
    The rate is sampled using the Poisson-mixture representation. A non-central chi-square variable is equivalent to a central chi-square variable with a random number of degrees of freedom:

    $$N \sim \text{Poisson}\left(\frac{\lambda}{2}\right), \quad r_{t+\Delta t} = c \cdot \chi^2_{d + 2N}$$

---
### 4. **Ho-Lee Model (HJM Framework)**

The Ho-Lee model is a "no-arbitrage" model that incorporates the current market yield curve to ensure the simulated rates are consistent with today's bond prices.

**Stochastic Differential Equation (SDE):**


$$dr(t) = \theta(t)dt + \sigma dW(t)$$

**Note:** `get_forward_rate(t)` ($f(0, t)$): Gets the instantaneous market forward rate.

**Numerical Scheme:**
To ensure the model fits the initial yield curve, the code calculates the time-dependent drift $\theta(t)$ using the slope of the forward curve and a volatility adjustment:


$$r_{t+\Delta t} = r_t + \underbrace{\left( \frac{\partial f(0, t)}{\partial t} + \sigma^2 t \right) \Delta t}_{\text{No-Arbitrage Drift}} + \sigma \sqrt{\Delta t} \cdot Z$$

**Implementation Details:**

* **Interpolated Spot Curve:** The function `get_interpolated_spot` uses linear interpolation to estimate rates between known market quotes, ensuring the model can be evaluated at any time $t$.
* **Forward Rate Derivation:** Since forward rates are usually not quoted directly, the code calculates them using the relationship:

$$f(0, t) = \frac{\partial}{\partial t} [r(0, t) \cdot t]$$

---

## Quick Start

**Compile:**
```bash
g++ method model_name.cpp
```

**Run:**
```bash
./model
```

## Applications

### Bond Pricing
All models provide closed-form or semi-analytical solutions for zero-coupon bond prices:

$$P(t, T) = \mathbb{E}^\mathbb{Q} \left[ \exp \left( -\int_t^T r(s) ds \right) \bigg| \mathcal{F}_t \right]$$


### Interest Rate Derivatives
- **Caps/Floors:** Option on interest rates
- **Swaptions:** Option to enter swap
- **Bond Options:** European options on bonds