//
// Created by xcao2 on 6/4/2025.
//

#ifndef OPTIONS_SIMULATOR_BAW_H
#define OPTIONS_SIMULATOR_BAW_H

#include "./shared/Option.h"

namespace BAW{
    double price(const Option& opt, int steps = 0);
}

#endif //OPTIONS_SIMULATOR_BAW_H
