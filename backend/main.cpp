#include <iostream>
#include <memory>
#include <chrono>
#include "EuropeanOption.h"
#include "OptionType.h"
#include "BlackScholes.h"
#include "BinomialTree.h"
#include "AmericanOption.h"

int main() {
    // Simulate market conditions
    double S = 100.0;
    double K = 101.0;
    double r = 0.04;
    double T = 1.5;
    double sigma = 0.1;

    auto start = std::chrono::high_resolution_clock::now();
    double val;
    auto end = std::chrono::high_resolution_clock::now();

    // ===== INHERITANCE TEST =====
    std::unique_ptr<Option> callOption = std::make_unique<EuropeanOptionInheritance>(S, K, r, sigma, T, Call);

    start = std::chrono::high_resolution_clock::now();
    val = callOption->price();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> duration_inheritance = end - start;

    // ===== TEMPLATE TEST =====
    EuropeanOption callOption_Template(S, K, r, sigma, T, Call);

    start = std::chrono::high_resolution_clock::now();
    val = BlackScholesGeneric<EuropeanOption>::price(callOption_Template);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> duration_template = end - start;

    // ==== TEMPLATE SEPARATE TEST ====
    EuropeanCallOption callOption_Separate(S, K, r, sigma, T);

    start = std::chrono::high_resolution_clock::now();
    val = BlackScholes<EuropeanCallOption>::price(callOption_Separate);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> duration_template_separate = end - start;

    // ==== TEMPLATE SEPARATE AMERICAN PUT TEST ====
    AmericanPutOption putOption_Separate(S, K, r, sigma, T);

    start = std::chrono::high_resolution_clock::now();
    val = BinomialTree<AmericanPutOption>::price(putOption_Separate);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> american_duration = end - start;


    // Output results
    std::cout << "Inheritance price():       " << duration_inheritance.count() << " ns\n";
    std::cout << "Template price():          " << duration_template.count() << " ns\n";
    std::cout << "Template Separate price(): " << duration_template_separate.count() << " ns\n";
    std::cout << "American BT price():       " << american_duration.count() << " ns\n";

    return 0;
}
