//
// Created by xcao2 on 6/1/2025.
//

#ifndef OPTIONS_SIMULATOR_TEMPLATEBENCHMARKS_H
#define OPTIONS_SIMULATOR_TEMPLATEBENCHMARKS_H

void benchmarkVirtualBlackScholes(int numOptions);
void benchmarkGenericBlackScholesTemplate(int numOptions);
void benchmarkSeparatedBlackScholes(int numOptions);
void benchmarkGenericBinomialTreeTemplate(int numOptions, int steps = 1000);
void benchmarkSeparatedBinomialTree(int numOptions, int steps = 1000);


#endif //OPTIONS_SIMULATOR_TEMPLATEBENCHMARKS_H
