#include <iostream>
#include <memory>
#include "EuropeanOption.h"
#include "shared/OptionType.h"

using namespace std;

int main() {
    //simulate market conditions
    double S = 100.0;
    double K = 101.0;
    double r = 0.04;
    double T = 1.5; //1.5 years
    double sigma = 0.1; // 10% volatility

    cout<<"Testing European Option Price & Greeks \n";

    std::unique_ptr<Option> callOption = std::make_unique<EuropeanOption>(S, K, r, sigma, T, Call);
    std::cout << "\nCall Option:\n";
    std::cout << "Price: " << callOption->price() << "\n";
    std::cout << "Delta: " << callOption->delta() << "\n";
    std::cout << "Gamma: " << callOption->gamma() << "\n";
    std::cout << "Vega : " << callOption->vega() << "\n";
    std::cout << "Theta: " << callOption->theta() << "\n";
    std::cout << "Rho  : " << callOption->rho() << "\n";

    // Put Option
    std::unique_ptr<Option> putOption = std::make_unique<EuropeanOption>(S, K, r, sigma, T, Put);
    std::cout << "\nPut Option:\n";
    std::cout << "Price: " << putOption->price() << "\n";
    std::cout << "Delta: " << putOption->delta() << "\n";
    std::cout << "Gamma: " << putOption->gamma() << "\n";
    std::cout << "Vega : " << putOption->vega() << "\n";
    std::cout << "Theta: " << putOption->theta() << "\n";
    std::cout << "Rho  : " << putOption->rho() << "\n";

    return 0;
}
