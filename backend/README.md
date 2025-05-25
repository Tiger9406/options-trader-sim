# Options Trading Simulator Backend

A performance-oriented C++ backend for a full-fledged options trading application. This backend 
currently simulates European options using the Black-Scholes model.

I hope to learn the mathematical reasoning behind options pricing, focusing on system design and financial realism.

---

## Current Features

- European call and put pricing via Black-Scholes
- Console interface
- OOP style options with inheritance for system design
- Template style instead of inheritance

## Previous Progression:
- European call & put as a class
- Insertion of inheritance anticipating types of options
- Moving to templates for faster resolving behavior at compile time, inline execution, no heap allocation, & no vtable lookup

## Project Structure

```
backend/
    CMakeLists.txt
    main.cpp
    README.md
    src/
        european/
            EuropeanOption.cpp
            EuropeanOption.h
        option/
            Option.h
        pricing/
            BlackScholes.h
        shared/
            OptionType.h
            MathUtil.h
```

## Functionality

### European
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

It calculates the "greeks," metrics used in options pricing using the Black Scholes model:
- Delta: rate of change of option price w/ respect to underlying/spot price
- Gamma: rate of change of Delta w/ respect to underlying price; second derivative
- Vega: Sensitivity of option price to 1% difference in sigma/volatility
- Theta: Sensitivity of option price w/ respect to passage of time: T decrease
- Rho: Sensitivity of option price to a 1% change in risk-free interest rate
