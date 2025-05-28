# Options Trader Simulator

This is a fullstack project for simulating options trading. I hope to learn the basics of 
**principals behind options pricing** as well as apply **efficient system design.**

The project consists of a high-performance C++ pricing engine that implements option pricing models (currently European 
options using the Black-Scholes formula and American option using the Binomial Tree Model), 
and will later include a frontend UI (React) and full portfolio/trading management features.

---

## Structure

- backend/: C++ pricing engine & (planned) simulation logic 
- frontend/: (planned) UI for user interaction and effective visualization
- data/: (planned) sample historical data for testing

---

## Status

### Completed (Reverse Chronological Order):
- Memory reuse for Binomial Tree (thread-local workspace)
- SIMD + OpenMP batch pricing (1M options in <20ms)
- Unified dispatcher for European & American pricing
- American Option pricing via Binomial Tree
- Performance Benchmarking
- Template-separated European Call/Put
- European Options Greeks (delta, gamma, etc.)
- C++ pricing engine complete for European call/put

### In Progress:
- American Greeks
- American options speedup: BAW and Ju-Zhong Models

### Planned:
- GPU acceleration (maybe)
- Portfolio simulation: track cash balance, option positions, etc. Methods like buy, sell 
- Market Simulation: Simulate realistic market conditions: volatility, price movements
- Strategies/Backtesting
- React frontend

---
## Performance Techniques
- **SIMD Vectorization**: Batch pricing leverages vectorized CPU instructions to price many options at once
- **Multithreading (OpenMP)**: Workloads are parallelized across CPU cores to increase throughput
- **Batch Processing**: Grouping options into SoA format for high-throughput vectorized pricing per CPU
- **Dispatcher Model**: Dynamically chooses between models based on option style (European vs American)

---
## Build & Run (C++ backend)

```bash
cd backend
mkdir build && cd build
cmake ..
make
./options_simulator
```
