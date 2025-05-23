#include <iostream>
#include "EuropeanOption.h"
using namespace std;

int main() {
    //simulate market conditions
    double S = 100.0;
    double K = 101.0;
    double r = 0.04;
    double Time = 1.5; //1.5 years
    double vol = 0.1; // 10% volatility

//    EuropeanOption callOption = EuropeanOption(S, K, r, Time, vol, OptionType::CALL);
    EuropeanOption callOption(S, K, r, Time, vol, OptionType::CALL);
    cout<<"European Call Option Price: " << callOption.getPrice()<<endl;

    EuropeanOption putOption(S, K, r, Time, vol, OptionType::PUT);
    cout<<"European Put Option Price: " <<putOption.getPrice()<<endl;

    return 0;
}
