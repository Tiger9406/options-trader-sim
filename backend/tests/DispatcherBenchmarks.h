//
// Created by xcao2 on 6/1/2025.
//

#ifndef OPTIONS_SIMULATOR_DISPATCHERBENCHMARKS_H
#define OPTIONS_SIMULATOR_DISPATCHERBENCHMARKS_H

#include "shared/Option.h"
#include <vector>
using AmericanPricerFn = double(*)(const Option&, int);

void benchmarkDispatcherSeparateStyle(int numEuropean, int numAmerican);
void benchmarkDispatcherMixedStyle(int numEuropean, int numAmerican);
std::vector<double> benchmarkParallelization(AmericanPricerFn americanPricer, int numEuropean = 0, int numAmerican = 0, std::vector<Option> allOptions = {});
void benchmarkBlackScholesSIMD(int numEuropean);

#endif //OPTIONS_SIMULATOR_DISPATCHERBENCHMARKS_H
