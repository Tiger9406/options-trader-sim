//
// Created by xcao2 on 6/4/2025.
//

#include "../include/BlackScholes.h"

double BlackScholes::price(const Option& opt){
    //assumes Brownian motion: develops randomly w/ constant volatility & constant drift rate/expected return
    //Z score describing how profitable option s
    double d1 = (std::log(opt.S / opt.K) + (opt.r - opt.q + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
    //Z score describing probability option to be executed
    double d2 = d1-opt.sigma*std::sqrt(opt.T);

    //dependent on option type
    if(opt.type == OptionType::Call) //call option price = CDF(d1)*spot_price - CDF(d2)*exercise_price*e^(-interest_rate*time_expiry)
        return std::exp(-opt.q*opt.T) * normCDF(d1)*opt.S-normCDF(d2)*opt.K*exp(-opt.r*opt.T);
    else
        return normCDF(-d2)*opt.K*exp(-opt.r*opt.T)-normCDF(-d1)*opt.S * std::exp(-opt.q*opt.T);
}
double BlackScholes::price(double S, double K, double r, double sigma, double T, double q, OptionType type){
    double d1 = (std::log(S/K)+(r+sigma*sigma/2)*T)/(sigma*std::sqrt(T));
    double d2 = d1-sigma*std::sqrt(T);

    if(type == OptionType::Call)
        return std::exp(-q * T)* normCDF(d1)*S-normCDF(d2)*K*exp(-r*T);
    else
        return normCDF(-d2)*K*exp(-r*T)-std::exp(-q * T)*normCDF(-d1)*S;
}

double BlackScholes::delta(const Option& opt) {
    double d1 = (std::log(opt.S / opt.K) + (opt.r - opt.q + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * sqrt(opt.T));
    return opt.type == Call ? std::exp(-opt.q * opt.T) * normCDF(d1) : std::exp(-opt.q * opt.T) * (normCDF(d1) - 1.0);
}

double BlackScholes::gamma(const Option& opt) {
    double d1 = (std::log(opt.S / opt.K) + (opt.r - opt.q + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
    return std::exp(-opt.q * opt.T) * normPDF(d1) / (opt.S * opt.sigma * std::sqrt(opt.T));
}

double BlackScholes::vega(const Option& opt) {
    double d1 = (std::log(opt.S / opt.K) + (opt.r - opt.q + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * sqrt(opt.T));
    return opt.S * std::exp(-opt.q * opt.T) * normPDF(d1) * std::sqrt(opt.T) / 100.0;
}

double BlackScholes::theta(const Option& opt) {
    double d1 = (std::log(opt.S / opt.K) + (opt.r - opt.q + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
    double d2 = d1 - opt.sigma * std::sqrt(opt.T);
    double term1 = -(opt.S * std::exp(-opt.q * opt.T) * normPDF(d1) * opt.sigma) / (2 * std::sqrt(opt.T));
    double term2 = opt.r * opt.K * std::exp(-opt.r * opt.T) * (opt.type == Call ? normCDF(d2) : normCDF(-d2));
    double term3 = opt.q * opt.S * std::exp(-opt.q * opt.T) * (opt.type == Call ? normCDF(d1) : normCDF(-d1));
    return (term1 - term2 + term3) / 365.0;
}

double BlackScholes::rho(const Option& opt) {
    double d2 = (std::log(opt.S / opt.K) + (opt.r - opt.q - 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
    return (opt.type == Call ?
            opt.K * opt.T * std::exp(-opt.r * opt.T) * normCDF(d2) :
            -opt.K * opt.T * std::exp(-opt.r * opt.T) * normCDF(-d2)) / 100.0;
}

Greeks BlackScholes::computeGreeks(const Option& opt) {
    double sqrtT = std::sqrt(opt.T);
    double qDiscount = std::exp(-opt.q * opt.T);
    double rDiscount = std::exp(-opt.r * opt.T);
    double sigmaSqrtT = opt.sigma * sqrtT;

    double d1 = (std::log(opt.S / opt.K) + (opt.r - opt.q + 0.5 * opt.sigma * opt.sigma) * opt.T) / (sigmaSqrtT);
    double d2 = d1 - sigmaSqrtT;

    double pdf_d1 = normPDF(d1);
    double cdf_d1 = normCDF(d1);
    double cdf_d2 = normCDF(d2);
    double cdf_nd1 = normCDF(-d1);
    double cdf_nd2 = normCDF(-d2);

    bool isCall = (opt.type == Call);

    // Delta
    double delta = qDiscount * (isCall ? cdf_d1 : cdf_d1 - 1.0);

    // Gamma
    double gamma = qDiscount * pdf_d1 / (opt.S * opt.sigma * sqrtT);

    // Vega
    double vega = opt.S * qDiscount * pdf_d1 * sqrtT / 100.0;

    // Theta
    double term1 = -(opt.S * qDiscount * pdf_d1 * opt.sigma) / (2.0 * sqrtT);
    double term2 = opt.r * opt.K * rDiscount * (isCall ? cdf_d2 : cdf_nd2);
    double term3 = opt.q * opt.S * qDiscount * (isCall ? cdf_d1 : cdf_nd1);
    double theta = (term1 - term2 + term3) / 365.0;

    // Rho
    double rho = (opt.K * opt.T * rDiscount * (isCall ? cdf_d2 : -cdf_nd2)) / 100.0;

    return { delta, gamma, theta, vega, rho };
}