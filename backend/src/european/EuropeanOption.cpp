//
// Created by xcao2 on 5/22/2025.
//

#include "EuropeanOption.h"
#include <cmath>
using namespace std;

EuropeanOption::EuropeanOption(double S_, double K_, double r_, double sigma_, double T_, OptionType type_)
        : Option(S_, K_, r_, sigma_, T_, type_) {}

double EuropeanOption::price() const {
    //assumes Brownian motion: develops randomly w/ constant volatility & constant drift rate/expected return
    //Z score describing how profitable option s
    double d1 = (log(S/K)+(r+sigma*sigma/2)*T)/(sigma*sqrt(T));
    //Z score describing probability option to be executed
    double d2 = d1-sigma*sqrt(T);

    //dependent on option type
    if(type == OptionType::Call) //call option price = CDF(d1)*spot_price - CDF(d2)*exercise_price*e^(-interest_rate*time_expiry)
        return normCDF(d1)*S-normCDF(d2)*K*exp(-r*T);
    else
        return normCDF(-d2)*K*exp(-r*T)-normCDF(-d1)*S;
}

double EuropeanOption::normCDF(double x) {
    //probability that random var following a normal distribution, <= given x
    return erfc(-x/sqrt(2))/2;
}

double EuropeanOption::normPDF(double x) {
    return (1/sqrt(2*M_PI))*exp(-x*x/2);
}

double EuropeanOption::delta() const {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    return type == Call ? normCDF(d1) : normCDF(d1) - 1.0;
}

double EuropeanOption::gamma() const {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    return normPDF(d1) / (S * sigma * std::sqrt(T));
}

double EuropeanOption::vega() const {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    return S * normPDF(d1) * std::sqrt(T) / 100.0;  // per 1% vol change
}

double EuropeanOption::theta() const {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    double term1 = -(S * normPDF(d1) * sigma) / (2 * std::sqrt(T));

    if (type == Call)
        return (term1 - r * K * std::exp(-r * T) * normCDF(d2)) / 365.0;
    else
        return (term1 + r * K * std::exp(-r * T) * normCDF(-d2)) / 365.0;
}

double EuropeanOption::rho() const {
    double d2 = (std::log(S / K) + (r - 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    if (type == Call)
        return K * T * std::exp(-r * T) * normCDF(d2) / 100.0;
    else
        return -K * T * std::exp(-r * T) * normCDF(-d2) / 100.0;
}

