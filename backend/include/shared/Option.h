//
// Created by xcao2 on 5/23/2025.
//

#ifndef OPTIONS_SIMULATOR_OPTION_H
#define OPTIONS_SIMULATOR_OPTION_H

#include "./OptionEnums.h"

struct Option{
    double S, K, r, sigma, T;
    OptionType type;
    OptionStyle style;

    Option(double S_, double K_, double r_, double sigma_, double T_,
           OptionType type_, OptionStyle style_)
            : S(S_), K(K_), r(r_), sigma(sigma_), T(T_),
              type(type_), style(style_) {}
};




#endif //OPTIONS_SIMULATOR_OPTION_H
