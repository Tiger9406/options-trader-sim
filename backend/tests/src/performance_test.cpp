#include "DispatcherBenchmarks.h"
#include "BinomialTree.h"
#include "BAW.h"
#include "BenchmarkUtils.h"




int main() {
    constexpr int NUM_EUROPEAN = 1'000'000;
    constexpr int NUM_AMERICAN = 1'000'000;


    std::vector<Option> allOptions = generateMixedOptions(0, NUM_AMERICAN);
//    std::vector<double> binomialPrices = benchmarkParallelization(&BinomialTree::price, 0, NUM_AMERICAN, allOptions);
    std::vector<double> BAWPrices = benchmarkParallelization(&BAW::price, 0, NUM_AMERICAN, allOptions);
//    std::vector<double> blackScholesPrice = benchmarkParallelization(&BAW::price, NUM_EUROPEAN, 0);
//    summarizePricingErrors(binomialPrices, BAWPrices, allOptions);

//    std::vector<double> BAW_BlackScholesPrices = benchmarkParallelization(&BAW::price, NUM_EUROPEAN, NUM_AMERICAN);

    return 0;
}