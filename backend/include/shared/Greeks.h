//
// Created by xcao2 on 5/29/2025.
//

#ifndef OPTIONS_SIMULATOR_OPTIOINRESULT_H
#define OPTIONS_SIMULATOR_OPTIOINRESULT_H

struct Greeks {
    double delta = 0.0;
    double gamma = 0.0;
    double theta = 0.0;
    double vega  = 0.0;
    double rho   = 0.0;
};

struct GreekResult {
    double price=0.0;
    Greeks greeks;
};


#endif //OPTIONS_SIMULATOR_OPTIOINRESULT_H
