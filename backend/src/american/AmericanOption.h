//
// Created by xcao2 on 5/25/2025.
//

#ifndef OPTIONS_SIMULATOR_AMERICANOPTION_H
#define OPTIONS_SIMULATOR_AMERICANOPTION_H

#include "../shared/OptionEnums.h"

struct AmericanOption {
    double S, K, T, r, sigma;
    OptionType type;

    AmericanOption(double S_, double K_, double r_, double sigma_, double T_, OptionType type_)
            : S(S_), K(K_), T(T_), r(r_), sigma(sigma_), type(type_) {}
};

struct AmericanCallOption {
    double S, K, T, r, sigma;
    static constexpr OptionType type = OptionType::Call;

    AmericanCallOption(double S_, double K_, double r_, double sigma_, double T_)
            : S(S_), K(K_), T(T_), r(r_), sigma(sigma_) {}
};

struct AmericanPutOption {
    double S, K, T, r, sigma;
    static constexpr OptionType type = OptionType::Put;

    AmericanPutOption(double S_, double K_, double r_, double sigma_, double T_)
            : S(S_), K(K_), T(T_), r(r_), sigma(sigma_) {}
};


#endif //OPTIONS_SIMULATOR_AMERICANOPTION_H
