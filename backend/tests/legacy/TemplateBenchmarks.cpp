//
// Created by xcao2 on 6/1/2025.
//

#include "TemplateBenchmarks.h"
#include "../BenchmarkUtils.h"
#include "legacy/EuropeanOption.h"
#include "legacy/AmericanOption.h"
#include "legacy/OptionClass.h"
#include "legacy/TemplatePricing.h"

#include <iostream>
#include <memory>

void benchmarkVirtualBlackScholes(int numOptions) {
    std::vector<std::unique_ptr<OptionClass>> options;
    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        options.emplace_back(std::make_unique<EuropeanOptionInheritance>(S, K, r, sigma, T, type));
    }

    std::cout << "\n[Virtual (Inheritance) Implementation]\n";
    double total = 0.0;
    total += benchmark("Price", [&]() { return applyAndSum(options, [](const auto& o) { return o->price(); }); });
    total += benchmark("Delta", [&]() { return applyAndSum(options, [](const auto& o) { return o->delta(); }); });
    total += benchmark("Gamma", [&]() { return applyAndSum(options, [](const auto& o) { return o->gamma(); }); });
    total += benchmark("Vega",  [&]() { return applyAndSum(options, [](const auto& o) { return o->vega(); }); });
    total += benchmark("Theta", [&]() { return applyAndSum(options, [](const auto& o) { return o->theta(); }); });
    total += benchmark("Rho",   [&]() { return applyAndSum(options, [](const auto& o) { return o->rho(); }); });
    std::cout << "Total: " << total << " ms\n";
}

void benchmarkGenericBlackScholesTemplate(int numOptions) {
    std::vector<EuropeanOption> options;
    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        options.emplace_back(S, K, r, sigma, T, type);
    }

    std::cout << "\n[Template-Based Black-Scholes]\n";
    double total = 0.0;
    total += benchmark("Price", [&]() { return applyAndSum(options, BlackScholesTemplate<EuropeanOption>::price); });
    total += benchmark("Delta", [&]() { return applyAndSum(options, BlackScholesTemplate<EuropeanOption>::delta); });
    total += benchmark("Gamma", [&]() { return applyAndSum(options, BlackScholesTemplate<EuropeanOption>::gamma); });
    total += benchmark("Vega",  [&]() { return applyAndSum(options, BlackScholesTemplate<EuropeanOption>::vega); });
    total += benchmark("Theta", [&]() { return applyAndSum(options, BlackScholesTemplate<EuropeanOption>::theta); });
    total += benchmark("Rho",   [&]() { return applyAndSum(options, BlackScholesTemplate<EuropeanOption>::rho); });
    std::cout << "Total: " << total << " ms\n";
}

void benchmarkSeparatedBlackScholes(int numOptions) {
    std::vector<EuropeanCallOption> calls;
    std::vector<EuropeanPutOption> puts;
    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        if (type == Call) calls.emplace_back(S, K, r, sigma, T);
        else puts.emplace_back(S, K, r, sigma, T);
    }

    std::cout << "\n[Separated Template-Based Black-Scholes]\n";
    double total = 0.0;
    total += benchmark("Call Price", [&]() { return applyAndSum(calls, BlackScholesSeparated<EuropeanCallOption>::price); });
    total += benchmark("Put Price",  [&]() { return applyAndSum(puts,  BlackScholesSeparated<EuropeanPutOption>::price); });
    std::cout << "Total: " << total << " ms\n";
}

void benchmarkGenericBinomialTreeTemplate(int numOptions, int steps) {
    std::vector<AmericanOption> options;
    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        options.emplace_back(S, K, r, sigma, T, type);
    }

    std::cout << "\n[Template-Based Binomial Tree]\n";
    benchmark("Price", [&]() {
        return applyAndSum(options, [&](const auto& o) {
            return BinomialTreeTemplate<AmericanOption>::price(o, steps);
        });
    });
}

void benchmarkSeparatedBinomialTree(int numOptions, int steps) {
    std::vector<AmericanCallOption> calls;
    std::vector<AmericanPutOption> puts;
    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        if (type == Call) calls.emplace_back(S, K, r, sigma, T);
        else puts.emplace_back(S, K, r, sigma, T);
    }

    std::cout << "\n[Separated Binomial Tree]\n";
    benchmark("Call Price", [&]() {
        return applyAndSum(calls, [&](const auto& o) {
            return BinomialTreeSeparated<AmericanCallOption>::price(o, steps);
        });
    });
    benchmark("Put Price", [&]() {
        return applyAndSum(puts, [&](const auto& o) {
            return BinomialTreeSeparated<AmericanPutOption>::price(o, steps);
        });
    });
}
