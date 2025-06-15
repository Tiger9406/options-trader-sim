//
// Created by xcao2 on 6/14/2025.
//

#ifndef OPTIONS_SIMULATOR_TESTUTILS_H
#define OPTIONS_SIMULATOR_TESTUTILS_H

#include <vector>
#include <Option.h>

void summarizePricingErrors(const std::vector<double>& prices1,
                            const std::vector<double>& prices2,
                            const std::vector<Option>& options);

#endif //OPTIONS_SIMULATOR_TESTUTILS_H
