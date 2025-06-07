//
// Created by xcao2 on 6/4/2025.
//

#include "../include/BinomialTree.h"
#include <vector>
#include "./shared/MathUtils.h"

double BinomialTree::price(const Option& opt, int steps) {
    double dt = opt.T / steps;
    double u =std::exp(opt.sigma*std::sqrt(dt));//up factor; stock price increase by factor of u
    double d = 1.0 /u; //down factor
    double p = (std::exp((opt.r - opt.q) * dt) - d) / (u - d); //risk neutral probability q
    double discount = std::exp(-opt.r * dt); //discount factor: to calculate how much money is worth today

    //at terminal, has total steps+1 possible prices
    //likelihood of reaching each terminal price parallel to combinatorics in flipping biased coin
    /* Reaching node with k up moves, N-k down moves:
     * Price S_k = S_0 * u^k * d^(N-k)
     * Probability = N choose k * q^k * (1-q)^(N-k) */
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
            double continuation = discount * (p * option_values[i] + (1 - p) * option_values[i + 1]); //val of option considering two possibilities
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

double BinomialTree::priceWorkspace(const Option &opt, int steps, BinomialWorkspace &workspace) {
    auto& prices = workspace.prices;
    auto& option_values = workspace.optionValues;

    double dt = opt.T / steps;
    double u = std::exp(opt.sigma * std::sqrt(dt));
    double d = 1.0 / u;
    double p = (std::exp((opt.r - opt.q) * dt) - d) / (u - d);
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
            double continuation = discount * (p * option_values[i] + (1 - p) * option_values[i + 1]);
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

double BinomialTree::priceParametersWorkspace(double S, double K, double r, double sigma, double T, double q,
                                              OptionType type, int steps, BinomialWorkspace &workspace) {
    auto& prices = workspace.prices;
    auto& option_values = workspace.optionValues;

    double dt = T / steps;
    double u = std::exp(sigma * std::sqrt(dt));
    double d = 1.0 / u;
    double p = (std::exp((r-q) * dt) - d) / (u - d);
    double discount = std::exp(-r * dt);
    for (int i = 0; i <= steps; i++) {
        prices[i] = S * std::pow(u, steps - i) * std::pow(d, i);
        if (type == OptionType::Call)
            option_values[i] = std::max(prices[i] - K, 0.0);
        else
            option_values[i] = std::max(K - prices[i], 0.0);
    }
    for (int step = steps - 1; step >= 0; step--) {
        for (int i = 0; i <= step; i++) {
            prices[i] /= u;
            double continuation = discount * (p * option_values[i] + (1 - p) * option_values[i + 1]);
            double exercise;
            if (type == OptionType::Call)
                exercise = std::max(prices[i] - K, 0.0);
            else
                exercise = std::max(K - prices[i], 0.0);
            option_values[i] = std::max(continuation, exercise);
        }
    }
    return option_values[0];
}

Greeks BinomialTree::computeGreeks(const Option& opt, BinomialWorkspace& workspace, int steps,
                                          double dS, double dT,
                                          double dSigma, double dR) {
    Option opt_up = opt; opt_up.S += dS;
    double price_up = priceWorkspace(opt_up, steps, workspace);

    Option opt_down = opt; opt_down.S -= dS;
    double price_down = priceWorkspace(opt_down, steps, workspace);

    Option opt_mid = opt;
    double price_mid = priceWorkspace(opt_mid, steps, workspace);

    // Delta & Gamma
    double delta = (price_up - price_down) / (2 * dS);
    double gamma = (price_up - 2 * price_mid + price_down) / (dS * dS);

    // Theta
    Option opt_early = opt; opt_early.T = std::max(opt.T - dT, 1e-8);
    double price_early = priceWorkspace(opt_early, steps, workspace);
    double theta = (price_early - price_mid) / dT;

    // Vega
    Option opt_vol = opt; opt_vol.sigma += dSigma;
    double price_vol = priceWorkspace(opt_vol, steps, workspace);
    double vega = (price_vol - price_mid) / dSigma;

    // Rho
    Option opt_r = opt; opt_r.r += dR;
    double price_r = priceWorkspace(opt_r, steps, workspace);
    double rho = (price_r - price_mid) / dR;

    return { delta, gamma, theta, vega, rho };
}