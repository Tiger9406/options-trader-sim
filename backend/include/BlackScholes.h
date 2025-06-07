//
// Created by xcao2 on 5/24/2025.
//

#ifndef OPTIONS_SIMULATOR_BLACKSCHOLES_H
#define OPTIONS_SIMULATOR_BLACKSCHOLES_H

#include <cmath>
#include "./shared/MathUtils.h"
#include "./shared/OptionEnums.h"
#include "./shared/Option.h"
#include "./shared/Greeks.h"

namespace BlackScholes{
    double price(const Option& opt);
    //overloaded to accept SoA
    double priceParameter(double S, double K, double r, double sigma, double T, double q, OptionType type);
    double delta(const Option& opt);
    double gamma(const Option& opt);
    double vega(const Option& opt);
    double theta(const Option& opt);
    double rho(const Option& opt);
    Greeks computeGreeks(const Option& opt);
};




#endif //OPTIONS_SIMULATOR_BLACKSCHOLES_H
