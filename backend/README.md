# Options Trading Simulator Backend

A performance-oriented C++ backend for a full-fledged options trading application. This backend 
currently simulates European options using the Black-Scholes model.

I hope to learn the mathematical reasoning behind options pricing, focusing on system design and financial realism.

---

## Current Features

- European call and put pricing via Black-Scholes
- Console interface

## Project Structure

```
backend/
    CMakeLists.txt
    main.cpp
    README.md
    src/
      EuropeanOption.cpp
      EuropeanOption.hpp
```

## Functionality

This simulator models a European-style option using:

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
