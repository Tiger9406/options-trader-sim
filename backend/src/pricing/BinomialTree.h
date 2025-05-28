//
// Created by xcao2 on 5/25/2025.
//

#ifndef OPTIONS_SIMULATOR_BINOMIALTREE_H
#define OPTIONS_SIMULATOR_BINOMIALTREE_H

#include <vector>
#include "../shared/MathUtils.h"
#include "../shared/OptionEnums.h"
#include "BinomialWorkspace.h"

class BinomialTree {
public:
    static double price(const Option& opt, int steps = 1000) {
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
    static double price(const Option& opt, int steps, BinomialWorkspace& workspace) {
        workspace.resize(steps);  // Ensure buffer is big enough
        auto& prices = workspace.prices;
        auto& option_values = workspace.optionValues;

        double dt = opt.T / steps;
        double u = std::exp(opt.sigma * std::sqrt(dt));
        double d = 1.0 / u;
        double q = (std::exp(opt.r * dt) - d) / (u - d);
        double discount = std::exp(-opt.r * dt);

        for (int i = 0; i <= steps; i++) {
            prices[i] = opt.S * std::pow(u, steps - i) * std::pow(d, i);
            if (opt.type == OptionType::Call)
                option_values[i] = std::max(prices[i] - opt.K, 0.0); //terminal option vals
            else
                option_values[i] = std::max(opt.K - prices[i], 0.0);
        }

        for (int step = steps - 1; step >= 0; step--) {
            for (int i = 0; i <= step; i++) {
                prices[i] /= u;
                double continuation = discount * (q * option_values[i] + (1 - q) * option_values[i + 1]);
                double exercise;
                if (opt.type == OptionType::Call)
                    exercise = std::max(prices[i] - opt.K, 0.0);
                else
                    exercise = std::max(opt.K - prices[i], 0.0);
                option_values[i] = std::max(continuation, exercise);
            }
        }

        return option_values[0];
    }

};


#endif //OPTIONS_SIMULATOR_BINOMIALTREE_H
