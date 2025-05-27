#ifndef OPTIONS_SIMULATOR_TEMPLATEPRICING_H
#define OPTIONS_SIMULATOR_TEMPLATEPRICING_H

#include <cmath>
#include "../shared/MathUtils.h"
#include "../shared/OptionEnums.h"
template<typename OptionTypeStruct>
class BlackScholesTemplate{
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


template<typename OptionTypeStruct>
class BlackScholesSeparated {
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
class BinomialTreeTemplate {
public:
    static double price(const OptionTypeStruct& opt, int steps = 1000) {
        double dt = opt.T / steps;
        //up factor; stock price increase by factor of u
        double u =std::exp(opt.sigma*std::sqrt(dt));
        //down factor
        double d = 1.0 /u;
        //risk neutral probability q
        double q = (std::exp(opt.r * dt)-d) / (u - d);
        //discount factor: to calculate how much money is worth today
        double discount = std::exp(-opt.r * dt);

        //at terminal, has total steps+1 possible prices
        //likelihood of reaching each terminal price parallel to combinatorics in flipping biased coin
        /*
         * Reaching node with k up moves, N-k down moves:
         * Price S_k = S_0 * u^k * d^(N-k)
         * Probability = N choose k * q^k * (1-q)^(N-k)
         */
        std::vector<double> prices(steps + 1);
        std::vector<double> option_values(steps + 1);

        // calculate terminal prices
        for (int i = 0; i <= steps; i++) {
            prices[i] = opt.S * std::pow(u, steps - i) * std::pow(d, i); //terminal stock price
            if (opt.type == OptionType::Call)
                option_values[i] = std::max(prices[i] - opt.K, 0.0); //terminal option vals
            else
                option_values[i] = std::max(opt.K - prices[i], 0.0);
        }

        // Backward induction: calculate option today by moving backwards
        for (int step = steps - 1; step >= 0; step--) {
            for (int i = 0; i <= step; i++) {
                prices[i] = prices[i] / u; //move prices back a step
                double continuation = discount * (q * option_values[i] + (1 - q) * option_values[i + 1]); //val of option considering two possibilities
                double exercise; //intrinsic val/early exercise val
                if (opt.type == OptionType::Call)
                    exercise = std::max(prices[i] - opt.K, 0.0);
                else
                    exercise = std::max(opt.K - prices[i], 0.0);
                //no arbitrage rule: pick higher of continuation and intrinsic
                option_values[i] = std::max(continuation, exercise);
            }
        }

        return option_values[0];
    }
};

template<typename OptionTypeStruct>
class BinomialTreeSeparated {
public:
    static double price(const OptionTypeStruct& opt, int steps = 1000) {
        double dt = opt.T / steps;
        //up factor; stock price increase by factor of u
        double u =std::exp(opt.sigma*std::sqrt(dt));
        //down factor
        double d = 1.0 /u;
        //risk neutral probability q
        double q = (std::exp(opt.r * dt)-d) / (u - d);
        //discount factor: to calculate how much money is worth today
        double discount = std::exp(-opt.r * dt);

        //at terminal, has total steps+1 possible prices
        //likelihood of reaching each terminal price parallel to combinatorics in flipping biased coin
        /*
         * Reaching node with k up moves, N-k down moves:
         * Price S_k = S_0 * u^k * d^(N-k)
         * Probability = N choose k * q^k * (1-q)^(N-k)
         */
        std::vector<double> prices(steps + 1);
        std::vector<double> option_values(steps + 1);

        // calculate terminal prices
        for (int i = 0; i <= steps; i++) {
            prices[i] = opt.S * std::pow(u, steps - i) * std::pow(d, i); //terminal stock price
            if constexpr (OptionTypeStruct::type == OptionType::Call)
                option_values[i] = std::max(prices[i] - opt.K, 0.0); //terminal option vals
            else
                option_values[i] = std::max(opt.K - prices[i], 0.0);
        }

        // Backward induction: calculate option today by moving backwards
        for (int step = steps - 1; step >= 0; step--) {
            for (int i = 0; i <= step; i++) {
                prices[i] = prices[i] / u; //move prices back a step
                double continuation = discount * (q * option_values[i] + (1 - q) * option_values[i + 1]); //val of option considering two possibilities
                double exercise; //intrinsic val/early exercise val
                if constexpr (OptionTypeStruct::type == OptionType::Call)
                    exercise = std::max(prices[i] - opt.K, 0.0);
                else
                    exercise = std::max(opt.K - prices[i], 0.0);
                //no arbitrage rule: pick higher of continuation and intrinsic
                option_values[i] = std::max(continuation, exercise);
            }
        }

        return option_values[0];
    }
};


#endif //OPTIONS_SIMULATOR_TEMPLATEPRICING_H