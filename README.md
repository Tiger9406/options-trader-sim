# Options Trader Simulator

This is a fullstack project for simulating options trading. I hope to learn the basics of 
principals behind options as well as focus on system design.
The project consists of a C++ backend that implements option pricing models (currently European 
options using the Black-Scholes formula and American option using the Binomial Tree Model), 
and will later include a frontend user interface(React) and portfolio/trading management features.

## Structure

- backend/: C++ pricing engine & (planned) simulation logic 
- frontend/: (planned) UI for user interaction and effective visualization
- tests/: general tests such as backend performance tests; could be restructured
- data/: (planned) sample historical data for testing

## Status

### Completed (Reverse Chronological Order):
- American Option pricing via Binomial Tree
- Separating Call & Put template for European
- Benchmarking
- Adapting into template style for European
- European Options Greeks
- C++ pricing engine complete for European call/put

### In Progress:
- American options speedup: BAW and Ju-Zhong Models

### Planned:
- Parallel calculations; CPU and maybe GPU
- Portfolio simulation: track cash balance, option positions, etc. Methods like buy, sell 
- Market Simulation: Simulate realistic market conditions: volatility, price movements
- Strategies/Backtesting
- React frontend


## Build & Run (C++ backend)

```bash
cd backend
mkdir build && cd build
cmake ..
make
./options_simulator
```