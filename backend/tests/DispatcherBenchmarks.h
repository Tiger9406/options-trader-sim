//
// Created by xcao2 on 6/1/2025.
//

#ifndef OPTIONS_SIMULATOR_DISPATCHERBENCHMARKS_H
#define OPTIONS_SIMULATOR_DISPATCHERBENCHMARKS_H

void benchmarkDispatcherSeparateStyle(int numEuropean, int numAmerican);
void benchmarkDispatcherMixedStyle(int numEuropean, int numAmerican);
void benchmarkParallelization(int numEuropean, int numAmerican);
void benchmarkBlackScholesSIMD(int numEuropean);

#endif //OPTIONS_SIMULATOR_DISPATCHERBENCHMARKS_H
