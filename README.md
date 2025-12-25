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
- American Option speedup: BAW
- Consideration of dividends
- Data Flow (I/O): Market Data Feed
- Transition to Linux (WSL)
- Memory reuse for Binomial Tree (thread-local workspace)
- SIMD + OpenMP batch pricing (1M options in <20ms)
- Unified dispatcher for European & American pricing
- American Option pricing & Greeks via Binomial Tree
- Performance Benchmarking
- Template-separated European Call/Put
- European Options Greeks (delta, gamma, etc.)
- C++ pricing engine complete for European call/put

### In Progress:
- American options speedup: Ju-Zhong Models

### Planned:
- Look into saving previously calculated Binomial Tree Prices,
avoid recalculation ever steps; have to fix ticks per unit time &
shift tree instead of recalculate
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
## Build & Run (C++ Backend on Linux with Boost.Asio + OpenMP)

```bash
# Enter your backend project directory
cd backend

# Create a build directory and run CMake
mkdir -p build && cd build
cmake ..

# Build with all cores
make -j$(nproc)

# if macbook
make -j$(sysctl -n hw.logicalcpu)

# Run the simulator
./options_simulator
```

### Requirements (Ubuntu/WSL)

Make sure you have the following dependencies installed:

```bash
sudo apt update
sudo apt install build-essential cmake libboost-all-dev
```

On Macbook:
```bash
brew upgrade
brew install cmake boost
```

This installs:
- GCC with OpenMP support
- Boost (including Boost.Asio)
- CMake for building the project
