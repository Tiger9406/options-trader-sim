//
// Created by xcao2 on 5/23/2025.
//

#ifndef OPTIONS_SIMULATOR_OPTION_H
#define OPTIONS_SIMULATOR_OPTION_H

#include "../shared/OptionType.h"

class Option {
protected:
    double S, K, r, sigma, T;
    OptionType type;
public:
    Option(double S_, double K_, double r_, double sigma_, double T_, OptionType type_) : S(S_), K(K_), r(r_), sigma(sigma_), T(T_), type(type_) {}

    virtual double price() const = 0;
    virtual double delta() const = 0;
    virtual double gamma() const = 0;
    virtual double vega() const = 0;
    virtual double theta() const = 0;
    virtual double rho() const = 0;

    virtual ~Option() = default;
};


#endif //OPTIONS_SIMULATOR_OPTION_H
