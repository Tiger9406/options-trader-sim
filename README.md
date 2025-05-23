# Options Trader Simulator

This is a fullstack project for simulating options trading. I hope to learn the basics of 
principals behind options as well as focus on system design.
The project consists of a C++ backend that implements option pricing models (currently European 
options using the Black-Scholes formula), and will later include a frontend user interface
(React) and portfolio/trading management features.

## Structure

- backend/: C++ pricing engine & (planned) simulation logic 
- frontend/: (planned) UI for user interaction and effective visualization
- data/: (planned) sample historical data for testing

## Status

### Completed:
- C++ pricing engine complete for European call/put

### In Progress:
- Greeks

### Planned:
- React frontend
- American options
- Portfolio simulation

## Build & Run (C++ backend)

```bash
cd backend
mkdir build && cd build
cmake ..
make
./options_simulator
```