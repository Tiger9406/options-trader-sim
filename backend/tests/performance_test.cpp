#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <memory>
#include "EuropeanOption.h"
#include "BlackScholes.h"
#include "OptionType.h"
#include "AmericanOption.h"
#include "BinomialTree.h"

// Utility function to measure and return execution time in milliseconds
template<typename Func>
double benchmark(const std::string& label, Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    double result = func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << label << " took " << duration.count() << " ms\n";
    return duration.count();
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

    double total_time = 0.0;

    total_time+=benchmark("Price", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholesGeneric<EuropeanOption>::price(opt);
        return sum;
    });

    total_time+=benchmark("Delta", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholesGeneric<EuropeanOption>::delta(opt);
        return sum;
    });

    total_time+=benchmark("Gamma", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholesGeneric<EuropeanOption>::gamma(opt);
        return sum;
    });

    total_time+=benchmark("Vega", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholesGeneric<EuropeanOption>::vega(opt);
        return sum;
    });

    total_time+=benchmark("Theta", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholesGeneric<EuropeanOption>::theta(opt);
        return sum;
    });

    total_time+=benchmark("Rho", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsTemplate) sum += BlackScholesGeneric<EuropeanOption>::rho(opt);
        return sum;
    });

    std::cout<< "Total Calculation Time: "<<total_time<<"\n";


    // ===== VIRTUAL BASED BENCHMARKS =====
    std::cout << "\n[Virtual (Inheritance) Implementation]\n";
    total_time=0.0;

    total_time+=benchmark("Price", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->price();
        return sum;
    });

    total_time+=benchmark("Delta", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->delta();
        return sum;
    });

    total_time+=benchmark("Gamma", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->gamma();
        return sum;
    });

    total_time+=benchmark("Vega", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->vega();
        return sum;
    });

    total_time+=benchmark("Theta", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->theta();
        return sum;
    });

    total_time+=benchmark("Rho", [&]() {
        double sum = 0.0;
        for (const auto& opt : optionsVirtual) sum += opt->rho();
        return sum;
    });
    std::cout<< "Total Calculation Time: "<<total_time<<"\n";
}

void benchmarkSeparatedCallPut(int numOptions) {
    std::vector<EuropeanCallOption> callOptions;
    std::vector<EuropeanPutOption> putOptions;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> spot(90, 110);
    std::uniform_real_distribution<double> strike(90, 110);
    std::uniform_real_distribution<double> rate(0.01, 0.05);
    std::uniform_real_distribution<double> vol(0.05, 0.3);
    std::uniform_real_distribution<double> expiry(0.1, 2.0);
    std::bernoulli_distribution typeDist(0.5);

    callOptions.reserve(numOptions / 2);
    putOptions.reserve(numOptions / 2);

    for (int i = 0; i < numOptions; ++i) {
        double S = spot(gen);
        double K = strike(gen);
        double r = rate(gen);
        double sigma = vol(gen);
        double T = expiry(gen);

        if (typeDist(gen))
            callOptions.emplace_back(S, K, r, sigma, T);
        else
            putOptions.emplace_back(S, K, r, sigma, T);
    }

    std::cout << "\n[Template Separated Call + Put Options]\n";

    double totalTime = 0.0;

    totalTime += benchmark("Price (Call)", [&]() {
        double sum = 0.0;
        for (const auto& opt : callOptions) sum += BlackScholes<EuropeanCallOption>::price(opt);
        return sum;
    });

    totalTime += benchmark("Price (Put)", [&]() {
        double sum = 0.0;
        for (const auto& opt : putOptions) sum += BlackScholes<EuropeanPutOption>::price(opt);
        return sum;
    });

    totalTime += benchmark("Delta (Call)", [&]() {
        double sum = 0.0;
        for (const auto& opt : callOptions) sum += BlackScholes<EuropeanCallOption>::delta(opt);
        return sum;
    });

    totalTime += benchmark("Delta (Put)", [&]() {
        double sum = 0.0;
        for (const auto& opt : putOptions) sum += BlackScholes<EuropeanPutOption>::delta(opt);
        return sum;
    });

    totalTime += benchmark("Gamma (Call)", [&]() {
        double sum = 0.0;
        for (const auto& opt : callOptions) sum += BlackScholes<EuropeanCallOption>::gamma(opt);
        return sum;
    });

    totalTime += benchmark("Gamma (Put)", [&]() {
        double sum = 0.0;
        for (const auto& opt : putOptions) sum += BlackScholes<EuropeanPutOption>::gamma(opt);
        return sum;
    });

    totalTime += benchmark("Vega (Call)", [&]() {
        double sum = 0.0;
        for (const auto& opt : callOptions) sum += BlackScholes<EuropeanCallOption>::vega(opt);
        return sum;
    });

    totalTime += benchmark("Vega (Put)", [&]() {
        double sum = 0.0;
        for (const auto& opt : putOptions) sum += BlackScholes<EuropeanPutOption>::vega(opt);
        return sum;
    });

    totalTime += benchmark("Theta (Call)", [&]() {
        double sum = 0.0;
        for (const auto& opt : callOptions) sum += BlackScholes<EuropeanCallOption>::theta(opt);
        return sum;
    });

    totalTime += benchmark("Theta (Put)", [&]() {
        double sum = 0.0;
        for (const auto& opt : putOptions) sum += BlackScholes<EuropeanPutOption>::theta(opt);
        return sum;
    });

    totalTime += benchmark("Rho (Call)", [&]() {
        double sum = 0.0;
        for (const auto& opt : callOptions) sum += BlackScholes<EuropeanCallOption>::rho(opt);
        return sum;
    });

    totalTime += benchmark("Rho (Put)", [&]() {
        double sum = 0.0;
        for (const auto& opt : putOptions) sum += BlackScholes<EuropeanPutOption>::rho(opt);
        return sum;
    });

    std::cout << "\n[Total Time for Separated Call + Put]: " << totalTime << " ms\n";
}

void benchmarkAmericanOptions(int numOptions, int steps = 1000){
    std::vector<AmericanPutOption> putOptions;
    putOptions.reserve(numOptions);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> spot(90, 110);
    std::uniform_real_distribution<double> strike(90, 110);
    std::uniform_real_distribution<double> rate(0.01, 0.05);
    std::uniform_real_distribution<double> vol(0.05, 0.3);
    std::uniform_real_distribution<double> expiry(0.1, 2.0);

    for (int i = 0; i < numOptions; ++i) {
        putOptions.emplace_back(
                spot(gen),
                strike(gen),
                rate(gen),
                vol(gen),
                expiry(gen)
        );
    }

    std::cout << "\n[American Put Options - Binomial Tree Implementation]\n";

    double totalTime = benchmark("Price (American Put)", [&]() {
        double sum = 0.0;
        for (const auto& opt : putOptions)
            sum += BinomialTree<AmericanPutOption>::price(opt, steps);
        return sum;
    });

    std::cout << "[Total Time for American Options]: " << totalTime << " ms\n";
}


int main() {
    constexpr int NUM_OPTIONS = 1'000'000;
    benchmarkBothImplementations(NUM_OPTIONS);
    benchmarkSeparatedCallPut(NUM_OPTIONS);
    benchmarkAmericanOptions(10000);
    return 0;
}
