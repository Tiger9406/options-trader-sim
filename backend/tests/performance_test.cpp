#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <memory>
#include <functional>
#include "EuropeanOption.h"
#include "BlackScholes.h"
#include "OptionEnums.h"
#include "AmericanOption.h"
#include "BinomialTree.h"
#include "PricingDispatcher.h"
#include "TemplatePricing.h"
#include "OptionBatch.h"
#include "BatchPricing.h"

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

// using applyAndSum do all prices and greeks; only for europeans as of now, legacy greeks not implemented
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
void benchmarkVirtualBlackScholes(int numOptions) {
    std::vector<std::unique_ptr<OptionClass>> optionsVirtual;

    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
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
}

// template generic black scholes
void benchmarkGenericBlackScholesTemplate(int numOptions) {
    std::vector<EuropeanOption> optionsTemplate;

    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        optionsTemplate.emplace_back(S, K, r, sigma, T, type);
    }

    std::cout << "\n[Template-Based Implementation]\n";
    double totalTemplate = benchmarkGreeks<decltype(optionsTemplate), BlackScholesTemplate<EuropeanOption>>(optionsTemplate, "", {});
    std::cout << "Total Calculation Time: " << totalTemplate << " ms\n";
}

// separate call & put european template; theoretically faster
void benchmarkSeparatedBlackScholes(int numOptions) {
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
    total += benchmarkGreeks<decltype(callOptions), BlackScholesSeparated<EuropeanCallOption>>(callOptions, "Call ", {});
    total += benchmarkGreeks<decltype(putOptions), BlackScholesSeparated<EuropeanPutOption>>(putOptions, "Put  ", {});
    std::cout << "Total Time for Separated Call + Put: " << total << " ms\n";
}

// legacy options
void benchmarkSeparatedBinomialTree(int numOptions, int steps = 1000) {
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

    std::cout << "\n[American Options - Separated Binomial Tree Implementation]\n";

    double totalTime = 0.0;
    totalTime += benchmark("Price (American Call)", [&]() {
        return applyAndSum(callOptions, [&](const AmericanCallOption& opt) {
            return BinomialTreeSeparated<AmericanCallOption>::price(opt, steps);
        });
    });

    totalTime += benchmark("Price (American Put)", [&]() {
        return applyAndSum(putOptions, [&](const AmericanPutOption& opt) {
            return BinomialTreeSeparated<AmericanPutOption>::price(opt, steps);
        });
    });

    std::cout << "Total Time for Separate Binomial Tree: " << totalTime << " ms\n";
}

// legacy options
void benchmarkGenericBinomialTreeTemplate(int numOptions, int steps = 1000) {
    std::vector<AmericanOption> americanOptions;

    RandomGenerator rng;
    for (int i = 0; i < numOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        americanOptions.emplace_back(S, K, r, sigma, T, type);
    }

    std::cout << "\n[American Options - Generic Binomial Tree Implementation]\n";

    double totalTime = 0.0;
    totalTime += benchmark("Price (Generic Binomial Tree)", [&]() {
        return applyAndSum(americanOptions, [&](const AmericanOption& opt) {
            return BinomialTreeTemplate<AmericanOption>::price(opt, steps);
        });
    });
}

//call Dispatcher::price on all options independently
void benchmarkDispatcherSeparateStyle(int numEuropean, int numAmerican) {
    std::vector<Option> europeanOptions;
    std::vector<Option> americanOptions;

    RandomGenerator rng;
    for (int i = 0; i < numEuropean; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        europeanOptions.emplace_back(S, K, r, sigma, T, type, OptionStyle::European);
    }
    for (int i = 0; i < numAmerican; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        americanOptions.emplace_back(S, K, r, sigma, T, type, OptionStyle::American);
    }

    std::cout << "\n[Unified Dispatcher-Based Benchmarking]\n";

    double totalTime = 0.0;

    totalTime += benchmark("Price (European via Dispatcher)", [&]() {
        return applyAndSum(europeanOptions, PricingDispatcher::price);
    });

    std::cout << "Total Time for Dispatcher-Based Black Scholes Pricing: " << totalTime << " ms\n";

    totalTime=0.0;
    totalTime += benchmark("Price (American via Dispatcher)", [&]() {
        return applyAndSum(americanOptions, PricingDispatcher::price);
    });

    std::cout << "Total Time for Dispatcher-Based Binomial Pricing: " << totalTime << " ms\n";
}
void benchmarkDispatcherMixedStyle(int numEuropean, int numAmerican) {
    std::vector<Option> allOptions;
    RandomGenerator rng;

    int totalOptions = numEuropean + numAmerican;
    std::vector<OptionStyle> styles;

    // Fill in the required number of each style
    styles.insert(styles.end(), numEuropean, OptionStyle::European);
    styles.insert(styles.end(), numAmerican, OptionStyle::American);

    // Shuffle the styles to mix the order
    std::shuffle(styles.begin(), styles.end(), std::mt19937{std::random_device{}()});

    // Generate options based on shuffled styles
    for (int i = 0; i < totalOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        allOptions.emplace_back(S, K, r, sigma, T, type, styles[i]);
    }

    std::cout << "\n[Unified Dispatcher-Based Benchmarking with Mixed Options]\n";

    double totalTime = 0.0;

    totalTime += benchmark("Price (All via Dispatcher)", [&]() {
        return applyAndSum(allOptions, PricingDispatcher::price);
    });

    std::cout << "Total Time for Dispatcher-Based Mixed Pricing: " << totalTime << " ms\n";
}

void benchmarkParallelization(int numEuropean, int numAmerican) {
    std::vector<Option> allOptions;
    RandomGenerator rng;

    int totalOptions = numEuropean + numAmerican;
    std::vector<OptionStyle> styles;

    // Fill in the required number of each style
    styles.insert(styles.end(), numEuropean, OptionStyle::European);
    styles.insert(styles.end(), numAmerican, OptionStyle::American);

    // Shuffle the styles to mix the order
    std::shuffle(styles.begin(), styles.end(), std::mt19937{std::random_device{}()});

    // Generate options based on shuffled styles
    for (int i = 0; i < totalOptions; ++i) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        allOptions.emplace_back(S, K, r, sigma, T, type, styles[i]);
    }

    std::cout << "\n[Unified Dispatcher-Based Benchmarking with Mixed Options using Parallelization]\n";

    auto start = std::chrono::high_resolution_clock::now();

    auto prices = PricingDispatcher::priceBatch(allOptions);
    auto end = std::chrono::high_resolution_clock::now();
    double totalTime = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Total Time for Dispatcher-Based Pricing: " << totalTime << " ms\n";
}

void benchmarkBlackScholesSIMD(int numEuropean){
    std::vector<Option> europeanOptions;
    RandomGenerator rng;
    // Generate options based on shuffled styles
    for (int i = 0; i < numEuropean; i++) {
        auto [S, K, r, sigma, T, type] = rng.generateOptionParams();
        europeanOptions.emplace_back(S, K, r, sigma, T, type, OptionStyle::European);
    }
    auto batch = toEuropeanBatch(europeanOptions);
    auto start = std::chrono::high_resolution_clock::now();
    auto prices = blackScholesBatch(batch);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Batch Black-Scholes pricing took: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
}

int main() {
    constexpr int NUM_EUROPEAN_OPTIONS = 1'000'000;
    //=============
    //Benchmarks for Inheritance, Template, Separated Template
    //=============
//    benchmarkVirtualBlackScholes(NUM_EUROPEAN_OPTIONS);
//    benchmarkGenericBlackScholesTemplate(NUM_EUROPEAN_OPTIONS);
//    benchmarkSeparatedBlackScholes(NUM_EUROPEAN_OPTIONS);

    constexpr int NUM_AMERICAN_OPTIONS = 10'000;
    //=============
    //Benchmarks for Template Binomial Tree
    //=============
//    benchmarkSeparatedBinomialTree(NUM_AMERICAN_OPTIONS); // Reduced for binomial tree speed
//    benchmarkGenericBinomialTreeTemplate(NUM_AMERICAN_OPTIONS);

    //=============
    //Benchmarks for dispatcher & observing time difference
    //in having separated European & American vs all together
    //=============
//    benchmarkDispatcherSeparateStyle(NUM_EUROPEAN_OPTIONS, NUM_AMERICAN_OPTIONS);
//    benchmarkDispatcherMixedStyle(NUM_EUROPEAN_OPTIONS, NUM_AMERICAN_OPTIONS);

    //=============
    //Batch processing: allow parallelization; significantly faster
    //=============
//    benchmarkParallelization(NUM_EUROPEAN_OPTIONS, NUM_AMERICAN_OPTIONS);
//    benchmarkBlackScholesSIMD(NUM_EUROPEAN_OPTIONS);
    return 0;
}
