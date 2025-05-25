#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <memory>
#include "EuropeanOption.h"
#include "BlackScholes.h"
#include "OptionType.h"

// Utility function to measure and return execution time in milliseconds
template<typename Func>
double benchmark(const std::string& label, Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    double result = func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << label << " took " << duration.count() << " ms\n";
    return result;
}

void benchmarkBothImplementations(int numOptions) {
    std::vector<EuropeanOption> optionsTemplate;
    std::vector<std::unique_ptr<Option>> optionsVirtual;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> spot(90, 110);
    std::uniform_real_distribution<double> strike(90, 110);
    std::uniform_real_distribution<double> rate(0.01, 0.05);
    std::uniform_real_distribution<double> vol(0.05, 0.3);
    std::uniform_real_distribution<double> expiry(0.1, 2.0);
    std::bernoulli_distribution typeDist(0.5);

    // Generate random market parameters
    std::vector<std::tuple<double, double, double, double, double, OptionType>> optionParams;
    optionParams.reserve(numOptions);

    for (int i = 0; i < numOptions; ++i) {
        optionParams.emplace_back(
                spot(gen),
                strike(gen),
                rate(gen),
                vol(gen),
                expiry(gen),
                typeDist(gen) ? Call : Put
        );
    }

    // === Construct Template Options ===
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& [S, K, r, sigma, T, type] : optionParams) {
        optionsTemplate.emplace_back(S, K, r, sigma, T, type);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> templateTime = end - start;
    std::cout << "[Construction] Template options took " << templateTime.count() << " ms\n";

    // === Construct Inheritance Options ===
    start = std::chrono::high_resolution_clock::now();
    for (const auto& [S, K, r, sigma, T, type] : optionParams) {
        optionsVirtual.emplace_back(std::make_unique<EuropeanOptionInheritance>(S, K, r, sigma, T, type));
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> inheritanceTime = end - start;
    std::cout << "[Construction] Inheritance options took " << inheritanceTime.count() << " ms\n";


    // ===== TEMPLATE BASED BENCHMARKS =====
    std::cout << "\n[Template-Based Implementation]\n";

    benchmark("Price", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholes<EuropeanOption>::price(opt);
        return sum;
    });

    benchmark("Delta", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholes<EuropeanOption>::delta(opt);
        return sum;
    });

    benchmark("Gamma", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholes<EuropeanOption>::gamma(opt);
        return sum;
    });

    benchmark("Vega", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholes<EuropeanOption>::vega(opt);
        return sum;
    });

    benchmark("Theta", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholes<EuropeanOption>::theta(opt);
        return sum;
    });

    benchmark("Rho", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholes<EuropeanOption>::rho(opt);
        return sum;
    });


    // ===== VIRTUAL BASED BENCHMARKS =====
    std::cout << "\n[Virtual (Inheritance) Implementation]\n";

    benchmark("Price", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->price();
        return sum;
    });

    benchmark("Delta", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->delta();
        return sum;
    });

    benchmark("Gamma", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->gamma();
        return sum;
    });

    benchmark("Vega", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->vega();
        return sum;
    });

    benchmark("Theta", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->theta();
        return sum;
    });

    benchmark("Rho", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->rho();
        return sum;
    });
}

int main() {
    constexpr int NUM_OPTIONS = 1'000'000;
    benchmarkBothImplementations(NUM_OPTIONS);
    return 0;
}
