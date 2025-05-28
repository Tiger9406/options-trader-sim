//
// Created by xcao2 on 5/27/2025.
//

#ifndef OPTIONS_SIMULATOR_BINOMIALWORKSPACE_H
#define OPTIONS_SIMULATOR_BINOMIALWORKSPACE_H

#include <vector>
//create once so don't have to reallocate space for every option
struct BinomialWorkspace {
    std::vector<double> prices;
    std::vector<double> optionValues;
    BinomialWorkspace(size_t steps) {
        resize(steps);
    }
    void resize(size_t steps) {
        prices.resize(steps + 1);
        optionValues.resize(steps + 1);
    }
};

#endif //OPTIONS_SIMULATOR_BINOMIALWORKSPACE_H
