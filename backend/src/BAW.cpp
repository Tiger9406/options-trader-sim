// Converted from online BAW implementation to match your style
#include "BAW.h"
#include "BlackScholes.h"
#include "OptionEnums.h"
#include "BinomialTree.h"
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
static double objective(double Sx, const Option& opt) {
    double K = opt.K, r = opt.r, q = opt.q, sigma = opt.sigma, T = opt.T;
    OptionType type = opt.type;

    if (Sx <= 0.0) return 1e100;

    double sigma2 = sigma*sigma;

    double n = calc_n(r, q, sigma2);
    double k = calc_k(r, T, sigma2);
    double d1 = (std::log(Sx / K) + (r - q + 0.5 * sigma2) * T) / (sigma * std::sqrt(T));

    if (type == Call) {
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
static double derivative(double Sx, const Option& opt, double h = 0.01) {
    return (objective(Sx + h, opt) - objective(Sx - h, opt)) / (2.0 * h);
}

static double second_derivative(double Sx, const Option& opt, double h = 0.01) {
    return (derivative(Sx + h, opt) - derivative(Sx - h, opt)) / (2.0 * h);
}

// Newton's method to find critical Sx
static double find_critical_Sx(const Option& opt, int maxIterations, double tol) {
    double Sx = opt.S;
    for (int i = 0; i < maxIterations; i++) {
        double f_prime = derivative(Sx, opt);
        double f_double_prime = second_derivative(Sx, opt);
        double newSx = Sx - f_prime / f_double_prime;
        if (std::abs(newSx - Sx) < tol) // on event difference is so minimal
            return newSx;
        Sx = newSx;
    }
    return -1; // fallback
}

// Final BAW price
double BAW::price(const Option& opt, int steps) {
    double S = opt.S, K = opt.K, r = opt.r, q = opt.q, sigma = opt.sigma, T = opt.T;
    OptionType type = opt.type;

    double sigma2 = sigma*sigma;

    double n = calc_n(r, q, sigma2);
    double k = calc_k(r, T, sigma2);

    int maxIterations = 200;
    double tol = 1e-5;
    double Sx = find_critical_Sx(opt, maxIterations, tol);
    if(Sx==-1){
//        std::cerr<<"Did not converge\n";
        return BinomialTree::price(opt, steps);
    }
    double d1 = (std::log(Sx / K) + (r - q + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));

    double euro = BlackScholes::price(opt);
    double amer = euro;

    if (type == Call) {
        double q2 = 0.5 * (-n + std::sqrt((n - 1.0) * (n - 1.0) + 4.0 * k));
        double A2 = Sx * (1.0 - std::exp(-q * T) * normCDF(d1)) / q2;
        amer = (S < Sx) ? euro + A2 * std::pow(S / Sx, q2) : std::max(0.0, S - K);
    } else {
        double q1 = 0.5 * (-n - std::sqrt((n - 1.0) * (n - 1.0) + 4.0 * k));
        double A1 = -Sx * (1.0 - std::exp(-q * T) * normCDF(-d1)) / q1;
        amer = (S > Sx) ? euro + A1 * std::pow(S / Sx, q1) : std::max(0.0, K - S);
    }

    return std::max(amer, euro);
}