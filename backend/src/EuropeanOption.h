//
// Created by xcao2 on 5/22/2025.
//

#ifndef OPTIONS_SIMULATOR_EUROPEANOPTION_H
#define OPTIONS_SIMULATOR_EUROPEANOPTION_H

//have to clearly distinguish between call & put
enum class OptionType {CALL, PUT};

class EuropeanOption {
public:
    EuropeanOption(double spot_price, double exercise_price, double interest_rate,
                   double time_expiry, double volatility, OptionType type);
    double getPrice() const;
private:
    double spot_price;
    double exercise_price;
    double interest_rate;
    double time_expiry;
    double volatility;
    OptionType type;
    double calculateBlackScholesPrice() const;
    double normCDF(double x) const;
};


#endif //OPTIONS_SIMULATOR_EUROPEANOPTION_H
