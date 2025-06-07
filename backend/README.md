# Options Trading Simulator Backend

A performance-oriented C++ backend for a full-fledged options trading application. This backend 
currently simulates European options using the Black-Scholes model and American options using 
Binomial Tree model.

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
    include/
        legacy/
            AmericanOption.h
            EuropeanOption.h
            OptionClass.h
            TempalatePricing.h
        shared/
            BinomialWorkspace.h
            Greeks.h
            MathUtils.h
            Option.h
            OptionBatch.h
            OptionEnums.h
        simulation/
            MarketDataFeed.cpp
            MarketDataFeed.h
        BinomialTree.h
        BlackScholes.h
        PricingDispatcher.h
    src/
        legacy/
            EuropeanOption.cpp
        PricingDispatcher.cpp
    tests/
        legacy/
            TemplateBenchmarks.cpp
            TemplateBenchmarks.h
        BenchmarkUtils.h
        DispatcherBenchmarks.cpp
        DispatcherBenchmarks.h
        performance_test.cpp
    CMakeLists.txt
    main.cpp
    README.md
```

## Functionality

### European Options (Analytical solution to Black-Scholes Model)
This simulator generates a price for European options using the solution to the Black-Scholes Model:
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
- Dividend rate (q)
#### Greeks Calculated
- **Delta**: sensitivity of option price w/ respect to underlying/spot price
- **Gamma**: sensitivity of Delta w/ respect to underlying price; second derivative
- **Vega**: Sensitivity of option price to sigma/volatility
- **Theta**: Sensitivity of option price to passage of time: T decrease
- **Rho**: Sensitivity of option price to risk-free interest rate

#### Performance Optimizations
- **SIMD Vectorization**: Batched pricing via converting to and using Structure of Arrays (SoA), enabling computation of vectorized prices at once
- **Multithreading (OpenMP)**: Parallelized across CPU cores for greater throughput
- **Batch API**: `blackScholesBatch()` computes 1M+ prices in <20ms on modern CPUs
- **Dispatch Model**: Runtime dispatcher falls back to scalar methods for mixed-style batches
- **Memory Reuse for American Options**: Binomial Tree model uses thread-local `BinomialWorkspace` to eliminate repeated vector allocations

### American
This simulator generates prices for American options using a Barone Whaley Model, which unlike European
options, can be exercised at any time before the expiration time.

#### Whaley
Whaley: good for very short maturities & good for very long maturities; extends the classic
Black-Scholes model, accounting for early exercise premium. It has O(n) time complexity.

- **Newton's Method**: iteratively find the critical stock price, where early exercise is optimal
- **Fallback**: If fails to converge, fallback to accurate, slower binomial tree (0.25% chance on randomly generated options)

#### Binomial Tree
Previously implemented; still used to compare to for accuracy. Extremely accurate but takes 
too long to compute, has a recursive nature to it, where it does backward induction for k steps,
resulting in an n^2 operation per option

- **Iterative Tree Construction**: Utilizes vector-based (not recursive) for speed and stability
- **Preallocated Buffers**: Reuses memory, reducing heap allocations in batch runs


### Benchmarked Performance
- **European Options (1M calls)**: ~12–20ms via SIMD & Parallelization
- **American Options (1M options)**: ~250-400ms with OpenMP & Parallelization for BAW
  - For Binomial Tree: 30,000ms-37,000 ms
- **Unified Mixed-Style Dispatcher**: Efficiently handles input with differing styles & types