//
// Created by xcao2 on 5/27/2025.
//

#ifndef OPTIONS_SIMULATOR_PRICINGDISPATCHER_H
#define OPTIONS_SIMULATOR_PRICINGDISPATCHER_H

#include "./BinomialTree.h"
#include "./BlackScholes.h"
#include "../shared/Option.h"
#include "../shared/BinomialWorkspace.h"
#include "./OptionBatch.h"
#include <stdexcept>
#include <cmath>
#include <vector>
#include <algorithm>

class PricingDispatcher {
public:
    //singular
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
    // barch pricing w/ parallelization
    static std::vector<double> priceBatch(const std::vector<Option>& opts, int steps=1000){
        std::vector<double> prices(opts.size());

        //allow parallelization across cpus; no vars shared
        #pragma omp parallel default(none) shared(opts, prices, steps)
        {
//            BinomialWorkspace workspace(steps); //declared inside for thread local; better at greater scale
            //split up
            #pragma omp for
            for (int i = 0; i < opts.size(); i++) {
                if (opts[i].style == OptionStyle::European) {
                    prices[i] = BlackScholes::price(opts[i]);
                } else {
                    prices[i] = BinomialTree::price(opts[i]);
                }
            }
        }
        return prices;
    }
    //only European
    static std::vector<double> priceBatchSIMD(const OptionBatch& batch) {
        size_t N = batch.size();
        std::vector<double> prices(N);

        double S, K, r, sigma, T, d1, d2;
        #pragma omp parallel for simd default(none) private(S, K, r, sigma, T, d1, d2) shared(batch, prices, N)
        for (size_t i = 0; i < N; i++) {
            S = batch.S[i], K = batch.K[i], r = batch.r[i], sigma = batch.sigma[i], T = batch.T[i];

            d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
            d2 = d1 - sigma * std::sqrt(T);

            if (batch.type[i] == OptionType::Call)
                prices[i] = normCDF(d1) * S - normCDF(d2) * K * std::exp(-r * T);
            else
                prices[i] = normCDF(-d2) * K * std::exp(-r * T) - normCDF(-d1) * S;
        }
        return prices;
    }
    //memory-reuse in Binomial Workspace; only american options
    static std::vector<double> priceBatchBinomialWorkspaceOnly(const std::vector<Option>& opts, int steps = 1000) {
        std::vector<double> prices(opts.size());
        #pragma omp parallel default(none) shared(opts, prices, steps)
        {
            BinomialWorkspace workspace(steps); // Thread-local
            #pragma omp for
            for (int i = 0; i < static_cast<int>(opts.size()); ++i) {
                prices[i] = BinomialTree::price(opts[i], steps, workspace);
            }
        }
        return prices;
    }
};

#endif //OPTIONS_SIMULATOR_PRICINGDISPATCHER_H
