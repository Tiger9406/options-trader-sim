//
// Created by xcao2 on 6/4/2025.
//

#include "BAW.h"
#include <cmath>
#include <algorithm>
#include "./shared/OptionEnums.h"
#include "../include/BlackScholes.h"
#include "../include/BinomialTree.h"
#include <iostream>

double BAW::price(const Option& opt, int steps){
    double S = opt.S, K = opt.K, r = opt.r, sigma = opt.sigma, T = opt.T, q = opt.q;
    OptionType type = opt.type;

    double european_price = BlackScholes::price(opt);
    if (type == OptionType::Call && S < K && european_price < 1.5) {
         std::cerr << "Warning: BAW fallback for deep OTM/low value Call. S=" << S << " K=" << K << " T=" << T << " sigma=" << sigma << " European=" << european_price << '\n';
        return BinomialTree::price(opt);
    }


    if (q >= r){
//        std::cerr <<"Warning, BAW assumption violated; utilizing binomial tree\n";
        return BinomialTree::price(opt);
    }

    double sigma2 = std::max(sigma * sigma, 1e-4); //in case of overly small sigma

    double beta, B_inf, B0, h, I, A;

    if (type == OptionType::Call) {
        beta = (0.5 - (r - q) / sigma2) + std::sqrt(std::pow((r - q) / sigma2 - 0.5, 2) + 2 * r / sigma2);
        if (!std::isfinite(beta) || beta <= 0.0){
//            std::cerr << "beta error with S=" << S << " K=" << K << " r=" << r << " q=" << q << " sigma=" << sigma << " T=" << T << '\n';
            return BinomialTree::price(opt);
        }

        B_inf = beta / (beta - 1.0) * K;
        B0 = std::max(K, r / (r - q) * K);
        h = -(r - q) * T + 2.0 * sigma * std::sqrt(T);
        I = B0 + (B_inf - B0) * (1.0 - std::exp(h)); //to find out where early exercise becomes optimal
        if (S >= I) //if already optimal
            return std::max(0.0, S - K); //return intrinsic val: exercise option now, immediate cash payout from exercising

        double european = BlackScholes::price(opt);
        A = (I - K) * std::pow(I, -beta);
        return std::max(0.0, european + A * std::pow(S, beta)); //european + early exercise premium

    } else {
        beta = (0.5 - (r - q) / sigma2) - std::sqrt(std::pow((r - q) / sigma2 - 0.5, 2) + 2 * r / sigma2);
        if (!std::isfinite(beta) || beta >= 0.0){
//            std::cerr << "beta error with S=" << S << " K=" << K << " r=" << r << " q=" << q << " sigma=" << sigma << " T=" << T << '\n';
            return BinomialTree::price(opt);
        }

        B_inf = beta / (beta - 1.0) * K;
        B0 = std::max(K, r / (r - q) * K);
        h = (r - q) * T - 2.0 * sigma * std::sqrt(T);
        I = B0 + (B_inf - B0) * (1.0 - std::exp(h)); //calculate bound

        if (S <= I)
            return std::max(0.0, K-S); //exercise now
        double european = BlackScholes::price(opt);
        A = (K - I) * std::pow(I, -beta);
        return std::max(0.0, european + A * std::pow(S, beta)); //min val is 0 bucks
    }
}