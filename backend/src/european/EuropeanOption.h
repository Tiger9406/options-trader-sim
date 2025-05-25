//
// Created by xcao2 on 5/22/2025.
//

#ifndef OPTIONS_SIMULATOR_EUROPEANOPTION_H
#define OPTIONS_SIMULATOR_EUROPEANOPTION_H

#include "../option/Option.h"
#include "../shared/OptionType.h"

class EuropeanOptionInheritance: public Option {
public:
    EuropeanOptionInheritance(double S_, double K_, double r_, double sigma_, double T_, OptionType type_);
    double price() const override;
    double delta() const override;
    double gamma() const override;
    double vega() const override;
    double theta() const override;
    double rho() const override;
};

struct EuropeanOption{
    double S, K, T, r, sigma;
    OptionType type;
    EuropeanOption(double S_, double K_, double r_, double sigma_, double T_, OptionType type_) : S(S_), K(K_), T(T_), r(r_), sigma(sigma_), type(type_){}
};
struct EuropeanCallOption {
    double S, K, T, r, sigma;
    static constexpr OptionType type = OptionType::Call;

    EuropeanCallOption(double S_, double K_, double r_, double sigma_, double T_)
            : S(S_), K(K_), T(T_), r(r_), sigma(sigma_) {}
};
struct EuropeanPutOption {
    double S, K, T, r, sigma;
    static constexpr OptionType type = OptionType::Put;

    EuropeanPutOption(double S_, double K_, double r_, double sigma_, double T_)
            : S(S_), K(K_), T(T_), r(r_), sigma(sigma_) {}
};


#endif //OPTIONS_SIMULATOR_EUROPEANOPTION_H
