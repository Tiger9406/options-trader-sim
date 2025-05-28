//
// Created by xcao2 on 5/27/2025.
//

#ifndef OPTIONS_SIMULATOR_PRICINGDISPATCHER_H
#define OPTIONS_SIMULATOR_PRICINGDISPATCHER_H

#include "./BinomialTree.h"
#include "./BlackScholes.h"
#include "../shared/Option.h"
#include <stdexcept>

class PricingDispatcher {
public:
    static double price(const Option& opt) {
        switch (opt.style) {
            case OptionStyle::European:
                return BlackScholes::price(opt);
            case OptionStyle::American:
                return BinomialTree::price(opt); // replace with BaroneAdesi later if needed
            default:
                throw std::invalid_argument("Unknown option style");
        }
    }
    static std::vector<double> priceBatch(const std::vector<Option>& opts) {
        std::vector<double> prices(opts.size());

        //allow parallelization across cpus
        #pragma omp parallel for default(none) shared(opts, prices)
        for (int i = 0; i < opts.size(); ++i)
            prices[i] = price(opts[i]);
        return prices;
    }
};

#endif //OPTIONS_SIMULATOR_PRICINGDISPATCHER_H
