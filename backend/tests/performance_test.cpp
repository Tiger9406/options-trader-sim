#include "DispatcherBenchmarks.h"

int main() {
    constexpr int NUM_EUROPEAN = 1'000'000;
    constexpr int NUM_AMERICAN = 10'000;

//    benchmarkDispatcherSeparateStyle(NUM_EUROPEAN, NUM_AMERICAN);
//    benchmarkDispatcherMixedStyle(NUM_EUROPEAN, NUM_AMERICAN);
    benchmarkParallelization(NUM_EUROPEAN, 0);
    benchmarkBlackScholesSIMD(NUM_EUROPEAN);

    return 0;
}