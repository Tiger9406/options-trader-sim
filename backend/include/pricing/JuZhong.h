//
// Created by xcao2 on 6/4/2025.
//

#ifndef OPTIONS_SIMULATOR_JUZHONG_H
#define OPTIONS_SIMULATOR_JUZHONG_H

#include "shared/OptionEnums.h"
#include "shared/Option.h"

namespace JuZhong {
    double price(const Option& opt, int steps = 1000);
};

#endif //OPTIONS_SIMULATOR_JUZHONG_H
