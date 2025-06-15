//
// Created by xcao2 on 5/25/2025.
//

#ifndef OPTIONS_SIMULATOR_BINOMIALTREE_H
#define OPTIONS_SIMULATOR_BINOMIALTREE_H

#include "shared/OptionEnums.h"
#include "shared/BinomialWorkspace.h"
#include "shared/Greeks.h"
#include "shared/Option.h"

namespace BinomialTree {
    double price(const Option& opt, int steps = 1000);
    //with additional workspace
    double priceWorkspace(const Option& opt, int steps, BinomialWorkspace& workspace);
    //overload for soa
    double priceParametersWorkspace(double S, double K, double r, double sigma, double T, double q, OptionType type, int steps, BinomialWorkspace& workspace);
    double priceParameters(double S, double K, double r, double sigma, double T, double q,
                    OptionType type, int steps);
    Greeks computeGreeks(const Option& opt, BinomialWorkspace& workspace, int steps = 1000,
                                double dS = 0.01, double dT = 1.0 / 365.0,
                                double dSigma = 0.01, double dR = 0.001);
};


#endif //OPTIONS_SIMULATOR_BINOMIALTREE_H
