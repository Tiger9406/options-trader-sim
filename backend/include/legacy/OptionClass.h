//
// Created by xcao2 on 5/29/2025.
//

#ifndef OPTIONS_SIMULATOR_OPTIONCLASS_H
#define OPTIONS_SIMULATOR_OPTIONCLASS_H
#include "shared/OptionEnums.h"

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

#endif //OPTIONS_SIMULATOR_OPTIONCLASS_H
