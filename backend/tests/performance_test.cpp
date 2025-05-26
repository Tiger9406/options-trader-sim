#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <memory>
#include <functional>
#include "EuropeanOption.h"
#include "BlackScholes.h"
#include "OptionType.h"
#include "AmericanOption.h"
#include "BinomialTree.h"

// Timing utility
template<typename Func>
double benchmark(const std::string& label, Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    double result = func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << label << " took " << duration.count() << " ms\n";
    return duration.count();
}

// given container, do sum of function over all items in container
template<typename Container, typename Func>
double applyAndSum(const Container& container, Func func) {
    double sum = 0.0;
    for (const auto& item : container) sum += func(item);
    return sum;
}

// using applyAndSum do all prices and greeks; only for europeans as of now, american greeks not implemented
template<typename Container, typename Engine>
double benchmarkGreeks(const Container& options, const std::string& prefix, Engine) {
    double total = 0.0;
    total += benchmark(prefix + "Price", [&]() { return applyAndSum(options, Engine::price); });
    total += benchmark(prefix + "Delta", [&]() { return applyAndSum(options, Engine::delta); });
    total += benchmark(prefix + "Gamma", [&]() { return applyAndSum(options, Engine::gamma); });
    total += benchmark(prefix + "Vega",  [&]() { return applyAndSum(options, Engine::vega); });
    total += benchmark(prefix + "Theta", [&]() { return applyAndSum(options, Engine::theta); });
    total += benchmark(prefix + "Rho",   [&]() { return applyAndSum(options, Engine::rho); });
    return total;
}

struct RandomGenerator {
    std::mt19937 gen;
    std::uniform_real_distribution<double> spot{90, 110};
    std::uniform_real_distribution<double> strike{90, 110};
    std::uniform_real_distribution<double> rate{0.01, 0.05};
    std::uniform_real_distribution<double> vol{0.05, 0.3};
    std::uniform_real_distribution<double> expiry{0.1, 2.0};
    std::bernoulli_distribution callPut{0.5};
    //construct
    RandomGenerator() : gen(std::random_device{}()) {}
    //to generate
    auto generateOptionParams() {
        return std::make_tuple(
                spot(gen),
                strike(gen),
                rate(gen),
                vol(gen),
                expiry(gen),
                callPut(gen) ? Call : Put
        );
    }
};
// template generic vs virtual
void benchmarkBothImplementations(int numOptions) {
    std::vector<EuropeanOption> optionsTemplate;
    std::vector<std::unique_ptr<Option>> optionsVirtual;

    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        optionsTemplate.emplace_back(S, K, r, sigma, T, type);
        optionsVirtual.emplace_back(std::make_unique<EuropeanOptionInheritance>(S, K, r, sigma, T, type));
    }

    std::cout << "\n[Virtual (Inheritance) Implementation]\n";
    double totalVirtual = 0.0;
    totalVirtual += benchmark("Price", [&]() { return applyAndSum(optionsVirtual, [](const auto& o) { return o->price(); }); });
    totalVirtual += benchmark("Delta", [&]() { return applyAndSum(optionsVirtual, [](const auto& o) { return o->delta(); }); });
    totalVirtual += benchmark("Gamma", [&]() { return applyAndSum(optionsVirtual, [](const auto& o) { return o->gamma(); }); });
    totalVirtual += benchmark("Vega",  [&]() { return applyAndSum(optionsVirtual, [](const auto& o) { return o->vega(); }); });
    totalVirtual += benchmark("Theta", [&]() { return applyAndSum(optionsVirtual, [](const auto& o) { return o->theta(); }); });
    totalVirtual += benchmark("Rho",   [&]() { return applyAndSum(optionsVirtual, [](const auto& o) { return o->rho(); }); });
    std::cout << "Total Calculation Time: " << totalVirtual << " ms\n";

    std::cout << "\n[Template-Based Implementation]\n";
    double totalTemplate = benchmarkGreeks<decltype(optionsTemplate), BlackScholesGeneric<EuropeanOption>>(optionsTemplate, "", {});
    std::cout << "Total Calculation Time: " << totalTemplate << " ms\n";
}

// separate call & put european template; theoretically faster
void benchmarkSeparatedCallPut(int numOptions) {
    std::vector<EuropeanCallOption> callOptions;
    std::vector<EuropeanPutOption> putOptions;

    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        if (type == Call)
            callOptions.emplace_back(S, K, r, sigma, T);
        else
            putOptions.emplace_back(S, K, r, sigma, T);
    }

    std::cout << "\n[Template Separated Call + Put Options]\n";
    double total = 0.0;
    total += benchmarkGreeks<decltype(callOptions), BlackScholes<EuropeanCallOption>>(callOptions, "Call ", {});
    total += benchmarkGreeks<decltype(putOptions), BlackScholes<EuropeanPutOption>>(putOptions, "Put  ", {});
    std::cout << "Total Time for Separated Call + Put: " << total << " ms\n";
}

// american options
void benchmarkAmericanOptions(int numOptions, int steps = 1000) {
    std::vector<AmericanCallOption> callOptions;
    std::vector<AmericanPutOption> putOptions;

    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        if (type == Call)
            callOptions.emplace_back(S, K, r, sigma, T);
        else
            putOptions.emplace_back(S, K, r, sigma, T);
    }

    std::cout << "\n[American Options - Binomial Tree Implementation]\n";

    double totalTime = 0.0;
    totalTime += benchmark("Price (American Call)", [&]() {
        return applyAndSum(callOptions, [&](const AmericanCallOption& opt) {
            return BinomialTree<AmericanCallOption>::price(opt, steps);
        });
    });

    totalTime += benchmark("Price (American Put)", [&]() {
        return applyAndSum(putOptions, [&](const AmericanPutOption& opt) {
            return BinomialTree<AmericanPutOption>::price(opt, steps);
        });
    });

    std::cout << "Total Time for American Options: " << totalTime << " ms\n";
}

int main() {
    constexpr int NUM_OPTIONS = 1'000'000;
    benchmarkBothImplementations(NUM_OPTIONS);
    benchmarkSeparatedCallPut(NUM_OPTIONS);
    benchmarkAmericanOptions(10'000); // Reduced for binomial tree speed
    return 0;
}
