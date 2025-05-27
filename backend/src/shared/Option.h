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

class OptionClass {
protected:
    double S, K, r, sigma, T;
    OptionType type;
public:
    OptionClass(double S_, double K_, double r_, double sigma_, double T_, OptionType type_) : S(S_), K(K_), r(r_), sigma(sigma_), T(T_), type(type_) {}

    virtual double price() const = 0;
    virtual double delta() const = 0;
    virtual double gamma() const = 0;
    virtual double vega() const = 0;
    virtual double theta() const = 0;
    virtual double rho() const = 0;

    virtual ~OptionClass() = default;
};


#endif //OPTIONS_SIMULATOR_OPTION_H
