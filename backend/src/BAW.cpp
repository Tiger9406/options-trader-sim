//
// Created by xcao2 on 6/4/2025.
//

#include "BAW.h"
#include <cmath>
#include <algorithm>
#include "./shared/OptionEnums.h"
#include "../include/BlackScholes.h"

double BAW::price(const Option& opt){
    double S = opt.S, K = opt.K, r = opt.r, sigma = opt.sigma, T = opt.T, q = opt.q;
    OptionType type = opt.type;

    if (type == OptionType::Call && q == 0.0) //american call on no dividends: little value of early exercise
        return BlackScholes::price(opt);

    double sigma2 = sigma * sigma;
    double beta = (0.5 - (r - q) / sigma2) + std::sqrt(std::pow((r - q) / sigma2 - 0.5, 2) + 2 * r / sigma2);

    double B_inf = beta / (beta - 1.0) * K;
    double B0 = std::max(K, r / (r - q) * K);
    double h = -(r - q) * T + 2.0 * sigma * std::sqrt(T);
    double I = B0 + (B_inf - B0) * (1.0 - std::exp(h)); //to find out where early exercise becomes optimal

    if (type == OptionType::Call) {
        if (S >= I) //if already optimal
            return S - K; //return intrinsic val: exercise option now, immediate cash payout from exercising
        double european = BlackScholes::price(opt);
        double A = (I - K) * std::pow(I, -beta);
        return european + A * std::pow(S, beta); //european + early exercise premium
    } else {
        beta = (0.5 - (r - q) / sigma2) -
               std::sqrt(std::pow((r - q) / sigma2 - 0.5, 2) + 2 * r / sigma2);
        B_inf = beta / (beta - 1.0) * K;
        B0 = std::max(K, r / (r - q) * K);
        h = (r - q) * T - 2.0 * sigma * std::sqrt(T);
        I = B0 + (B_inf - B0) * (1.0 - std::exp(h)); //calculate bound

        if (S <= I)
            return K - S;
        double european = BlackScholes::price(opt);
        double A = (K - I) * std::pow(I, -beta);
        return european + A * std::pow(S, beta);
    }
}