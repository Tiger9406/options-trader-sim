//
// Created by xcao2 on 5/22/2025.
//

#include "EuropeanOption.h"
#include <cmath>
using namespace std;

EuropeanOption::EuropeanOption(double spot_price, double exercise_price, double interest_rate, double time_expiry,
                               double volatility, OptionType type) :
                               spot_price(spot_price), exercise_price(exercise_price),interest_rate(interest_rate),
                               time_expiry(time_expiry), volatility(volatility), type(type) {}

double EuropeanOption::getPrice() const { return calculateBlackScholesPrice(); }

double EuropeanOption::calculateBlackScholesPrice() const {
    //assumes Brownian motion: develops randomly w/ constant volatility & constant drift rate/expected return
    //Z score describing how profitable option s
    double d1 = (log(spot_price/exercise_price)+(interest_rate+volatility*volatility/2)*time_expiry)/(volatility*sqrt(time_expiry));
    //Z score describing probability option to be executed
    double d2 = d1-volatility*sqrt(time_expiry);

    //dependent on option type
    if(type == OptionType::CALL) //call option price = CDF(d1)*spot_price - CDF(d2)*exercise_price*e^(-interest_rate*time_expiry)
        return normCDF(d1)*spot_price-normCDF(d2)*exercise_price*exp(-interest_rate*time_expiry);
    else
        return normCDF(-d2)*exercise_price*exp(-interest_rate*time_expiry)-normCDF(-d1)*spot_price;
}

double EuropeanOption::normCDF(double x) const {
    //probability that random var following a normal distribution, <= given x
    return erfc(-x/sqrt(2))/2;
}