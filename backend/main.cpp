#include <iostream>
#include <memory>
#include <chrono>
#include "EuropeanOption.h"
#include "OptionType.h"
#include "BlackScholes.h"

int main() {
    //simulate market conditions
    double S = 100.0;
    double K = 101.0;
    double r = 0.04;
    double T = 1.5; //1.5 years
    double sigma = 0.1; // 10% volatility

    std::cout<<"Testing European Option Price & Greeks Inheritance\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto val= 0.0;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::unique_ptr<Option> callOption = std::make_unique<EuropeanOptionInheritance>(S, K, r, sigma, T, Call);
    std::cout << "\nCall Option:\n";

    start = std::chrono::high_resolution_clock::now();
    val = callOption->price();
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Price: " << val << "\n";
    std::cout << "Operation took " << duration.count() << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    val = callOption->delta();
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Delta: " << val << "\n";
    std::cout << "Operation took " << duration.count() << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    val = callOption->gamma();
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Gamma: " << val << "\n";
    std::cout << "Operation took " << duration.count() << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    val = callOption->vega();
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Vega: " << val << "\n";
    std::cout << "Operation took " << duration.count() << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    val = callOption->theta();
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Theta: " << val << "\n";
    std::cout << "Operation took " << duration.count() << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    val = callOption->rho();
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Rho: " << val << "\n";
    std::cout << "Operation took " << duration.count() << " milliseconds." << std::endl;

    // Put Option
    std::unique_ptr<Option> putOption = std::make_unique<EuropeanOptionInheritance>(S, K, r, sigma, T, Put);
    std::cout << "\nPut Option:\n";
    std::cout << "Price: " << putOption->price() << "\n";
    std::cout << "Delta: " << putOption->delta() << "\n";
    std::cout << "Gamma: " << putOption->gamma() << "\n";
    std::cout << "Vega : " << putOption->vega() << "\n";
    std::cout << "Theta: " << putOption->theta() << "\n";
    std::cout << "Rho  : " << putOption->rho() << "\n";

    std::cout<<"Testing European Option Price & Greeks Template\n";

    EuropeanOption callOption_Template(S, K, r, sigma, T, Call);
    EuropeanOption putOption_Template(S, K, r, sigma, T, Put);
    std::cout << "\nCall Option:\n";

    start = std::chrono::high_resolution_clock::now();
    val = BlackScholes<EuropeanOption>::price(callOption_Template);
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Price: " << val << "\n";
    std::cout << "Operation took " << duration.count() << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    val = BlackScholes<EuropeanOption>::price(callOption_Template);
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Price: " << val << "\n";
    std::cout << "Operation took " << duration.count() << " milliseconds." << std::endl;

    std::cout << "Delta: " << BlackScholes<EuropeanOption>::delta(callOption_Template) << "\n";
    std::cout << "Gamma: " << BlackScholes<EuropeanOption>::gamma(callOption_Template) << "\n";
    std::cout << "Vega : " << BlackScholes<EuropeanOption>::vega(callOption_Template) << "\n";
    std::cout << "Theta: " << BlackScholes<EuropeanOption>::theta(callOption_Template) << "\n";
    std::cout << "Rho  : " << BlackScholes<EuropeanOption>::rho(callOption_Template) << "\n";

    std::cout << "\nPut Option:\n";
    std::cout << "Price: " << BlackScholes<EuropeanOption>::price(putOption_Template) << "\n";
    std::cout << "Delta: " << BlackScholes<EuropeanOption>::delta(putOption_Template) << "\n";
    std::cout << "Gamma: " << BlackScholes<EuropeanOption>::gamma(putOption_Template) << "\n";
    std::cout << "Vega : " << BlackScholes<EuropeanOption>::vega(putOption_Template) << "\n";
    std::cout << "Theta: " << BlackScholes<EuropeanOption>::theta(putOption_Template) << "\n";
    std::cout << "Rho  : " << BlackScholes<EuropeanOption>::rho(putOption_Template) << "\n";



    return 0;
}
