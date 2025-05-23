//
// Created by xcao2 on 5/22/2025.
//

#ifndef OPTIONS_SIMULATOR_EUROPEANOPTION_H
#define OPTIONS_SIMULATOR_EUROPEANOPTION_H

#include "option.h"

class EuropeanOption: public Option {
public:
    EuropeanOption(double S_, double K_, double r_, double sigma_, double T_, OptionType type_);
    double price() const override;
    double delta() const override;
    double gamma() const override;
    double vega() const override;
    double theta() const override;
    double rho() const override;
private:
    static double normCDF(double x) ;
    static double normPDF(double x) ;
};


#endif //OPTIONS_SIMULATOR_EUROPEANOPTION_H
