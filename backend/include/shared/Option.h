//
// Created by xcao2 on 5/23/2025.
//

#ifndef OPTIONS_SIMULATOR_OPTION_H
#define OPTIONS_SIMULATOR_OPTION_H

#include "./OptionEnums.h"

struct Option{
    double S, K, r, sigma, T, q;
    OptionType type;
    OptionStyle style;

    Option(double S_, double K_, double r_, double sigma_, double T_, double q_,
           OptionType type_, OptionStyle style_)
            : S(S_), K(K_), r(r_), sigma(sigma_), T(T_), q(q_),
              type(type_), style(style_) {}
    Option() : S(0.0), K(0.0), r(0.0), sigma(0.0), T(0.0), q(0.0),
               type(OptionType::Call), style(OptionStyle::European) {}
};




#endif //OPTIONS_SIMULATOR_OPTION_H
