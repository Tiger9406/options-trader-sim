//
// Created by xcao2 on 5/27/2025.
//

#ifndef OPTIONS_SIMULATOR_PRICINGDISPATCHER_H
#define OPTIONS_SIMULATOR_PRICINGDISPATCHER_H

#include <vector>
#include "./BinomialTree.h"
#include "./BlackScholes.h"
#include "./shared/Option.h"
#include "./shared/BinomialWorkspace.h"
#include "./shared/OptionBatch.h"
using AmericanPricerFn = double(*)(const Option&, int);

class PricingDispatcher {
public:
    //General
    static double price(const Option& opt);
    static std::vector<double> priceParallelized(const std::vector<Option>& opts, AmericanPricerFn americanPricer, int steps=1000);
    static std::vector<double> priceBatch(const OptionBatch& batch, int steps = 1000);

    //test specific methods
    static std::vector<double> priceBatchBlackScholesSIMD(const OptionBatch& batch);
    static std::vector<double> priceBatchBinomialWorkspace(const std::vector<Option>& opts, int steps = 1000);

    static std::vector<GreekResult> priceAndGreeks(const std::vector<Option>& opts, int steps = 1000);
    std::vector<Greeks> greeks(const std::vector<Option>& opts, int steps = 1000);

};

#endif //OPTIONS_SIMULATOR_PRICINGDISPATCHER_H
