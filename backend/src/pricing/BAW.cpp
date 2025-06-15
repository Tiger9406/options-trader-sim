// Converted from online BAW implementation to match your style
#include "pricing/BAW.h"
#include "pricing/BlackScholes.h"
#include "shared/OptionEnums.h"
#include "pricing/BinomialTree.h"
#include <cmath>
#include <vector>
#include <iostream>

// Compute q1/q2 constants for critical stock price calculation
static double calc_n(double r, double q, double sigma2) {
    return 2.0 * (r - q) / (sigma2);
}

static double calc_k(double r, double T, double sigma2) {
    return 2.0 * r / (sigma2 * (1.0 - std::exp(-r * T)));
}

// Objective function for Newton's method to find Sx
static double objective(double Sx, double S, double K, double r, double q, double sigma, double T, OptionType type) {
    if (Sx <= 0.0) return 1e100;

    double sigma2 = sigma*sigma;

    double n = calc_n(r, q, sigma2);
    double k = calc_k(r, T, sigma2);
    double d1 = (std::log(Sx / K) + (r - q + 0.5 * sigma2) * T) / (sigma * std::sqrt(T));

    if (type == OptionType::Call) {
        double q2 = 0.5 * (-n + std::sqrt((n - 1.0) * (n - 1.0) + 4.0 * k));
        return std::pow(
                BlackScholes::priceParameter(Sx, K, r, sigma, T, q, Call) +
                (1.0 - std::exp(-q * T) * normCDF(d1)) * Sx / q2 -
                Sx + K, 2.0
        );
    } else {
        double q1 = 0.5 * (-n - std::sqrt((n - 1.0) * (n - 1.0) + 4.0 * k));
        return std::pow(
                BlackScholes::priceParameter(Sx, K, r, sigma, T, q, Put) -
                (1.0 - std::exp(-q * T) * normCDF(-d1)) * Sx / q1 +
                Sx - K, 2.0
        );
    }
}

// Numerical derivative for Newton's method
static double derivative(double Sx, double S, double K, double r, double q, double sigma, double T, OptionType type, double h = 0.01) {
    return (objective(Sx + h, S, K, r, q, sigma, T, type) - objective(Sx - h, S, K, r, q, sigma, T, type)) / (2.0 * h);
}

static double second_derivative(double Sx, double S, double K, double r, double q, double sigma, double T, OptionType type, double h = 0.01) {
    return (derivative(Sx + h, S, K, r, q, sigma, T, type) - derivative(Sx - h, S, K, r, q, sigma, T, type)) / (2.0 * h);
}
//static int baw_fallback_counter = 0;
// Newton's method to find critical Sx
static double find_critical_Sx(double S, double K, double r, double q, double sigma, double T, OptionType type, int maxIterations, double tol) {
    double Sx = S;
    for (int i = 0; i < maxIterations; i++) {
        double f_prime = derivative(Sx, S, K, r, q, sigma, T, type);
        double f_double_prime = second_derivative(Sx, S, K, r, q, sigma, T, type);
        double newSx = Sx - f_prime / f_double_prime;
        if (std::abs(newSx - Sx) < tol) // on event difference is so minimal
            return newSx;
        Sx = newSx;
    }
//    std::cerr << "Fallback "<<baw_fallback_counter<<"\n";
//    ++baw_fallback_counter;
    return -1;
}

// Final BAW price
double BAW::price(const Option& opt, int steps) {
    double S = opt.S, K = opt.K, r = opt.r, q = opt.q, sigma = opt.sigma, T = opt.T;
    OptionType type = opt.type;

    double sigma2 = sigma*sigma;

    double n = calc_n(r, q, sigma2);
    double k = calc_k(r, T, sigma2);

    int maxIterations = steps;
    double tol = 1e-5;
    double Sx = find_critical_Sx(S, K, r, q, sigma, T, type, maxIterations, tol);
    if(Sx==-1){
//        std::cerr<<"Did not converge\n";
        return BinomialTree::price(opt, steps);
    }
    double d1 = (std::log(Sx / K) + (r - q + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));

    double euro = BlackScholes::price(opt);
    double amer = euro;

    if (type == OptionType::Call) {
        double q2 = 0.5 * (-n + std::sqrt((n - 1.0) * (n - 1.0) + 4.0 * k));
        double A2 = Sx * (1.0 - std::exp(-q * T) * normCDF(d1)) / q2;
        amer = (S < Sx) ? euro + A2 * std::pow(S / Sx, q2) : std::max(0.0, S - K);
    } else {
        double q1 = 0.5 * (-n - std::sqrt((n - 1.0) * (n - 1.0) + 4.0 * k));
        double A1 = -Sx * (1.0 - std::exp(-q * T) * normCDF(-d1)) / q1;
        amer = (S > Sx) ? euro + A1 * std::pow(S / Sx, q1) : std::max(0.0, K - S);
    }

    return amer;
}

double BAW::priceParameters(double S, double K, double r, double sigma, double T, double q, OptionType type, int steps){
    double sigma2 = sigma*sigma;

    double n = calc_n(r, q, sigma2);
    double k = calc_k(r, T, sigma2);

    int maxIterations = steps;
    double tol = 1e-5;
    double Sx = find_critical_Sx(S, K, r, q, sigma, T, type, maxIterations, tol);
    if(Sx==-1){
//        std::cerr<<"Did not converge\n";
        return BinomialTree::priceParameters(S, K, r, sigma, T, q, type, 1000);
    }
    double d1 = (std::log(Sx / K) + (r - q + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));

    double euro = BlackScholes::priceParameter(S, K, r, sigma, T, q, type);
    double amer = euro;

    if (type == OptionType::Call) {
        double q2 = 0.5 * (-n + std::sqrt((n - 1.0) * (n - 1.0) + 4.0 * k));
        double A2 = Sx * (1.0 - std::exp(-q * T) * normCDF(d1)) / q2;
        amer = (S < Sx) ? euro + A2 * std::pow(S / Sx, q2) : std::max(0.0, S - K);
    } else {
        double q1 = 0.5 * (-n - std::sqrt((n - 1.0) * (n - 1.0) + 4.0 * k));
        double A1 = -Sx * (1.0 - std::exp(-q * T) * normCDF(-d1)) / q1;
        amer = (S > Sx) ? euro + A1 * std::pow(S / Sx, q1) : std::max(0.0, K - S);
    }

    return amer;
}