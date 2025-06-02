//
// Created by xcao2 on 6/1/2025.
//

#include "DispatcherBenchmarks.h"
#include "BenchmarkUtils.h"
#include "shared/OptionBatch.h"
#include "PricingDispatcher.h"

#include <chrono>
#include <iostream>
#include <vector>

void benchmarkDispatcherSeparateStyle(int numEuropean, int numAmerican) {
    std::vector<Option> europeanOptions = generateOptions(numEuropean, OptionStyle::European);
    std::vector<Option> americanOptions = generateOptions(numAmerican, OptionStyle::American);

    std::cout << "\n[Unified Dispatcher-Based Benchmarking]\n";
    benchmark("Price (European via Dispatcher)", [&]() {
        return applyAndSum(europeanOptions, PricingDispatcher::price);
    });

    benchmark("Price (American via Dispatcher)", [&]() {
        return applyAndSum(americanOptions, PricingDispatcher::price);
    });
}

void benchmarkDispatcherMixedStyle(int numEuropean, int numAmerican) {
    std::vector<Option> mixedOptions = generateMixedOptions(numEuropean, numAmerican);

    benchmark("Price (Mixed via Dispatcher)", [&]() {
        return applyAndSum(mixedOptions, PricingDispatcher::price);
    });
}

void benchmarkParallelization(int numEuropean, int numAmerican) {
    std::vector<Option> allOptions = generateMixedOptions(numEuropean, numAmerican);
    auto start = std::chrono::high_resolution_clock::now();
    auto prices = PricingDispatcher::priceParallelized(allOptions);
    auto end = std::chrono::high_resolution_clock::now();
    double totalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Total Time (Parallelized Dispatcher): " << totalTime << " ms\n";
}

void benchmarkBlackScholesSIMD(int numEuropean) {
    std::vector<Option> europeanOptions = generateOptions(numEuropean, OptionStyle::European);
    auto batch = toBatch(europeanOptions);
    auto start = std::chrono::high_resolution_clock::now();
    auto prices = PricingDispatcher::priceBatchSIMD(batch);
    auto end = std::chrono::high_resolution_clock::now();
    double totalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Total Time (Black-Scholes SIMD): " << totalTime << " ms\n";
}