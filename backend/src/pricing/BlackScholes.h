//
// Created by xcao2 on 5/24/2025.
//

#ifndef OPTIONS_SIMULATOR_BLACKSCHOLES_H
#define OPTIONS_SIMULATOR_BLACKSCHOLES_H

#include <cmath>
#include "../shared/MathUtils.h"
#include "../shared/OptionType.h"

template<typename OptionTypeStruct>
class BlackScholes {
public:
    static double price(const OptionTypeStruct& opt) {
        double d1 = (std::log(opt.S / opt.K) + (opt.r + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
        double d2 = d1 - opt.sigma * std::sqrt(opt.T);

        if constexpr (OptionTypeStruct::type == OptionType::Call)
            return normCDF(d1) * opt.S - normCDF(d2) * opt.K * std::exp(-opt.r * opt.T);
        else
            return normCDF(-d2) * opt.K * std::exp(-opt.r * opt.T) - normCDF(-d1) * opt.S;
    }

    static double delta(const OptionTypeStruct& opt) {
        double d1 = (std::log(opt.S / opt.K) + (opt.r + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
        if constexpr (OptionTypeStruct::type == OptionType::Call)
            return normCDF(d1);
        else
            return normCDF(d1) - 1.0;
    }

    static double gamma(const OptionTypeStruct& opt) {
        double d1 = (std::log(opt.S / opt.K) + (opt.r + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
        return normPDF(d1) / (opt.S * opt.sigma * std::sqrt(opt.T));
    }

    static double vega(const OptionTypeStruct& opt) {
        double d1 = (std::log(opt.S / opt.K) + (opt.r + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
        return opt.S * normPDF(d1) * std::sqrt(opt.T) / 100.0;
    }

    static double theta(const OptionTypeStruct& opt) {
        double d1 = (std::log(opt.S / opt.K) + (opt.r + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
        double d2 = d1 - opt.sigma * std::sqrt(opt.T);
        double term1 = -(opt.S * normPDF(d1) * opt.sigma) / (2 * std::sqrt(opt.T));

        if constexpr (OptionTypeStruct::type == OptionType::Call)
            return (term1 - opt.r * opt.K * std::exp(-opt.r * opt.T) * normCDF(d2)) / 365.0;
        else
            return (term1 + opt.r * opt.K * std::exp(-opt.r * opt.T) * normCDF(-d2)) / 365.0;
    }

    static double rho(const OptionTypeStruct& opt) {
        double d2 = (std::log(opt.S / opt.K) + (opt.r - 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));

        if constexpr (OptionTypeStruct::type == OptionType::Call)
            return opt.K * opt.T * std::exp(-opt.r * opt.T) * normCDF(d2) / 100.0;
        else
            return -opt.K * opt.T * std::exp(-opt.r * opt.T) * normCDF(-d2) / 100.0;
    }
};

template<typename OptionTypeStruct>
class BlackScholesGeneric{
public:
    static double price(const OptionTypeStruct& opt){
        //assumes Brownian motion: develops randomly w/ constant volatility & constant drift rate/expected return
        //Z score describing how profitable option s
        double d1 = (std::log(opt.S/opt.K)+(opt.r+opt.sigma*opt.sigma/2)*opt.T)/(opt.sigma*std::sqrt(opt.T));
        //Z score describing probability option to be executed
        double d2 = d1-opt.sigma*std::sqrt(opt.T);

        //dependent on option type
        if(opt.type == OptionType::Call) //call option price = CDF(d1)*spot_price - CDF(d2)*exercise_price*e^(-interest_rate*time_expiry)
            return normCDF(d1)*opt.S-normCDF(d2)*opt.K*exp(-opt.r*opt.T);
        else
            return normCDF(-d2)*opt.K*exp(-opt.r*opt.T)-normCDF(-d1)*opt.S;
    }
    static double delta(const OptionTypeStruct& opt){
        double d1 = (std::log(opt.S / opt.K) + (opt.r + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * sqrt(opt.T));
        return opt.type == Call ? normCDF(d1) : normCDF(d1) - 1.0;
    }
    static double gamma(const OptionTypeStruct& opt){
        double d1 = (std::log(opt.S / opt.K) + (opt.r + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
        return normPDF(d1) / (opt.S * opt.sigma * std::sqrt(opt.T));
    }
    static double vega(const OptionTypeStruct& opt){
        double d1 = (std::log(opt.S / opt.K) + (opt.r + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * sqrt(opt.T));
        return opt.S * normPDF(d1) * std::sqrt(opt.T) / 100.0;  // per 1% vol change
    }
    static double theta(const OptionTypeStruct& opt){
        double d1 = (std::log(opt.S / opt.K) + (opt.r + 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
        double d2 = d1 - opt.sigma * std::sqrt(opt.T);
        double term1 = -(opt.S * normPDF(d1) * opt.sigma) / (2 * std::sqrt(opt.T));

        if (opt.type == Call)
            return (term1 - opt.r * opt.K * std::exp(-opt.r * opt.T) * normCDF(d2)) / 365.0;
        else
            return (term1 + opt.r * opt.K * std::exp(-opt.r * opt.T) * normCDF(-d2)) / 365.0;
    }
    static double rho(const OptionTypeStruct& opt){
        double d2 = (std::log(opt.S / opt.K) + (opt.r - 0.5 * opt.sigma * opt.sigma) * opt.T) / (opt.sigma * std::sqrt(opt.T));
        if (opt.type == Call)
            return opt.K * opt.T * std::exp(-opt.r * opt.T) * normCDF(d2) / 100.0;
        else
            return -opt.K * opt.T * std::exp(-opt.r * opt.T) * normCDF(-d2) / 100.0;
    }
};


#endif //OPTIONS_SIMULATOR_BLACKSCHOLES_H
