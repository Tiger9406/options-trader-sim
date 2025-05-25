//
// Created by xcao2 on 5/24/2025.
//

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include "EuropeanOption.h"
#include "OptionType.h"
#include "BlackScholes.h"

constexpr int NUM_OPTIONS = 10000;

int main() {
    double S = 100, K = 100, r = 0.05, sigma = 0.2, T = 1.0;

    // --- Inheritance-based creation ---
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::unique_ptr<Option>> opts_inherit;
    for (int i = 0; i < NUM_OPTIONS; ++i) {
        OptionType type = (i % 2 == 0) ? Call : Put;
        opts_inherit.push_back(std::make_unique<EuropeanOptionInheritance>(S, K, r, sigma, T, type));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "[INHERITANCE] Creation time: "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    // --- Template-based creation ---
    start = std::chrono::high_resolution_clock::now();
    std::vector<EuropeanOption> opts_template;
    for (int i = 0; i < NUM_OPTIONS; ++i) {
        OptionType type = (i % 2 == 0) ? OptionType::Call : OptionType::Put;
        opts_template.emplace_back(S, K, T, r, sigma, type);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[TEMPLATE]    Creation time: "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    // --- Price calculation ---
    start = std::chrono::high_resolution_clock::now();
    for (auto& opt : opts_inherit) {
        volatile double price = opt->price();
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[INHERITANCE] Price calc time: "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    for (const auto& opt : opts_template) {
        volatile double price = BlackScholes<EuropeanOption>::price(opt);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[TEMPLATE]    Price calc time: "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    // --- Greeks calculation ---
    start = std::chrono::high_resolution_clock::now();
    for (auto& opt : opts_inherit) {
        volatile double delta = opt->delta();
        volatile double gamma = opt->gamma();
        volatile double vega  = opt->vega();
        volatile double theta = opt->theta();
        volatile double rho   = opt->rho();
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[INHERITANCE] Greeks calc time: "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    for (const auto& opt : opts_template) {
        volatile double delta = BlackScholes<EuropeanOption>::delta(opt);
        volatile double gamma = BlackScholes<EuropeanOption>::gamma(opt);
        volatile double vega  = BlackScholes<EuropeanOption>::vega(opt);
        volatile double theta = BlackScholes<EuropeanOption>::theta(opt);
        volatile double rho   = BlackScholes<EuropeanOption>::rho(opt);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[TEMPLATE]    Greeks calc time: "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    return 0;
}
