//
// Created by xcao2 on 5/24/2025.
//

#ifndef OPTIONS_SIMULATOR_MATHUTILS_H
#define OPTIONS_SIMULATOR_MATHUTILS_H

#include <cmath>

inline double normCDF(double x){
    //probability that random var following a normal distribution, <= given x
    if (x < -6.0) return 0.0;
    if (x > 6.0) return 1.0;

    static const double a1 = 0.254829592;
    static const double a2 = -0.284496736;
    static const double a3 = 1.421413741;
    static const double a4 = -1.453152027;
    static const double a5 = 1.061405429;
    static const double p = 0.3275911;

    double sign = (x >= 0) ? 1.0 : -1.0;
    x = std::abs(x);

    double t = 1.0 / (1.0 + p * x);
    double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * std::exp(-x * x);

    return 0.5 * (1.0 + sign * y);
}

inline double normPDF(double x){
    return (1/sqrt(2*M_PI))*exp(-x*x/2);
}

#endif //OPTIONS_SIMULATOR_MATHUTILS_H
