#include <iostream>
#include <memory>
#include <chrono>
#include "EuropeanOption.h"
#include "OptionType.h"
#include "BlackScholes.h"

int main() {
    // Simulate market conditions
    double S = 100.0;
    double K = 101.0;
    double r = 0.04;
    double T = 1.5;
    double sigma = 0.1;

    // ===== INHERITANCE TEST =====
    std::unique_ptr<Option> callOption = std::make_unique<EuropeanOptionInheritance>(S, K, r, sigma, T, Call);

    auto start = std::chrono::high_resolution_clock::now();
    double val_inheritance = callOption->price();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> duration_inheritance = end - start;

    // ===== TEMPLATE TEST =====
    EuropeanOption callOption_Template(S, K, r, sigma, T, Call);

    start = std::chrono::high_resolution_clock::now();
    double val_template = BlackScholesGeneric<EuropeanOption>::price(callOption_Template);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> duration_template = end - start;

    // ==== TEMPLATE SEPARATE TEST ====
    EuropeanCallOption callOption_Separate(S, K, r, sigma, T);

    start = std::chrono::high_resolution_clock::now();
    double val_template_separate = BlackScholes<EuropeanCallOption>::price(callOption_Separate);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> duration_template_separate = end - start;


    // Output results
    std::cout << "Inheritance price():       " << duration_inheritance.count() << " ns\n";
    std::cout << "Template price():          " << duration_template.count() << " ns\n";
    std::cout << "Template Separate price(): " << duration_template_separate.count() << " ns\n";

    return 0;
}
