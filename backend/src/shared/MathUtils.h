//
// Created by xcao2 on 5/24/2025.
//

#ifndef OPTIONS_SIMULATOR_MATHUTILS_H
#define OPTIONS_SIMULATOR_MATHUTILS_H

#include <cmath>

inline double normCDF(double x){
    //probability that random var following a normal distribution, <= given x
    return erfc(-x/sqrt(2))/2;
}

inline double normPDF(double x){
    return (1/sqrt(2*M_PI))*exp(-x*x/2);
}

#endif //OPTIONS_SIMULATOR_MATHUTILS_H
