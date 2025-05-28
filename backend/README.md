# Options Trading Simulator Backend

A performance-oriented C++ backend for a full-fledged options trading application. This backend 
currently simulates European options using the Black-Scholes model.

I hope to learn the mathematical reasoning behind options pricing, focusing on system design and financial realism.

---

## Features
- Current:
  - Price dispatcher: given option, return price
    - European call and put pricing via Black-Scholes
    - American call and put pricing via Binomial Tree
  - Comprehensive performance test to evaluate computation speed on millions of options
- Out of Date (still in codebase):
  - OOP Inheritance style; override functions (slow)
  - Generic European Option Template (saves time but rigid)
  - Separate Call & Put Options (saves time but rigid)

## Previous Progression:
- European call & put as a class
- Insertion of inheritance anticipating types of options
- Moving to templates for faster resolving behavior at compile time, inline execution, no heap allocation, & no vtable lookup
- Separate call and put options for reduced branching for template
  - Observe minimal increase speed
- Opt for class-based option with composition of type & style

## Project Structure

```
backend/
    benchmarks/
       BENCHMARKS.md
       legacy_benchmarks.txt 
    src/
        pricing/
            BatchPricing.h
            BinomialTree.h
            BlackScholes.h
            OptionBatch.h
            PricingDispatcher.h
            TemplatePricing.h
        shared/
            Option.h
            MathUtil.h
            OptionEnums.h
        legacy/
            EuropeanOption.cpp
            EuropeanOption.h
            AmericanOption.h
    tests/
        performance_test.cpp
    CMakeLists.txt
    main.cpp
    README.md
```

## Functionality

### European Options (Black-Scholes Model)
This simulator generates a price for European options using the Black-Scholes Model:
```
Call = S * N(d1) - K * e^(-rT) * N(d2)
```

where N(d) is the cumulative distribution function of the standard normal distribution.
The model calculates fair value based on:

- Underlying stock price (S)
- Strike price (K)
- Time to expiration (T)
- Risk-free rate (r)
- Volatility (σ)
#### Greeks Calculated
- **Delta**: sensitivity of option price w/ respect to underlying/spot price
- **Gamma**: sensitivity of Delta w/ respect to underlying price; second derivative
- **Vega**: Sensitivity of option price to sigma/volatility
- **Theta**: Sensitivity of option price to passage of time: T decrease
- **Rho**: Sensitivity of option price to risk-free interest rate

#### Performance Optimizations
- **SIMD Vectorization**: Batched pricing via converting to and using Structure of Arrays (SoA), enabling computation of vectorized prices at once
- **Multithreading (OpenMP)**: Parallelized across CPU cores for greater throughput.
- **Batch API**: `blackScholesBatch()` computes 1M+ prices in <20ms on modern CPUs.
- **Dispatch Model**: Runtime dispatcher falls back to scalar methods for mixed-style batches.

Having undergone several implementations of this, concluded that separate types (Calls, Puts) Templates
is fastest and most logical in large scale.

### American
This simulator generates prices for American options using a Binomial Tree model, which unlike European
options, can be exercised at any time before the expiration time.

#### Model Features
- **Iterative Tree Construction**: Utilizes vector-based (not recursive) for speed and stability

### Benchmarked Performance
- **European Options (1M calls)**: ~18–20ms
- **American Options (10k options)**: ~400ms with OpenMP & Parallelization
- **Unified Mixed-Style Dispatcher**: Efficiently handles input with differing styles & types