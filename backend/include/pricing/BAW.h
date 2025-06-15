//
// Created by xcao2 on 6/4/2025.
//

#ifndef OPTIONS_SIMULATOR_BAW_H
#define OPTIONS_SIMULATOR_BAW_H

#include "shared/Option.h"

namespace BAW{
    double price(const Option& opt, int steps = 100);
    double priceParameters(double S, double K, double r, double sigma, double T, double q, OptionType type, int steps = 100);
}

#endif //OPTIONS_SIMULATOR_BAW_H
