//
// Created by xcao2 on 5/29/2025.
//

#include "pricing/PricingDispatcher.h"
#include "pricing/BlackScholes.h"
#include "pricing/BinomialTree.h"
#include "shared/BinomialWorkspace.h"
#include "pricing/BAW.h"
#include <cmath>
#include <stdexcept>
#include <omp.h>

//singular
double PricingDispatcher::price(const Option& opt) {
    switch (opt.style) {
        case OptionStyle::European:
            return BlackScholes::price(opt);
        case OptionStyle::American:
            return BinomialTree::price(opt); // replace with BaroneAdesi later if needed
        default:
            throw std::invalid_argument("Unknown option style");
    }
}

// barch include w/ parallelization
std::vector<double> PricingDispatcher::priceParallelized(const std::vector<Option>& opts, AmericanPricerFn americanPricer, int steps){
    std::vector<double> prices(opts.size());

    //allow parallelization across cpus; no vars shared
    #pragma omp parallel default(none) shared(opts, prices, steps, americanPricer)
    {
        //split up
        #pragma omp for
        for (int i = 0; i < opts.size(); i++) {
            if (opts[i].style == OptionStyle::European) {
                prices[i] = BlackScholes::price(opts[i]);
            } else {
                prices[i] = americanPricer(opts[i], steps);
            }
        }
    }
    return prices;
}
//for memory locality, avoid edit every object
std::vector<double> PricingDispatcher::priceBatch(const OptionBatch& batch, int steps) {
    size_t N = batch.size();
    std::vector<double> prices(N);

    #pragma omp parallel default(none) shared(batch, prices, steps, N)
    {
        BinomialWorkspace workspace(steps);  // per-thread workspace

    #pragma omp for
        for (int i = 0; i < N; ++i) {
            if (batch.style[i] == OptionStyle::European)
                prices[i] = BlackScholes::priceParameter(batch.S[i], batch.K[i], batch.r[i], batch.sigma[i], batch.T[i], batch.q[i], batch.type[i]);
            else
                prices[i] = BAW::priceParameters(batch.S[i], batch.K[i], batch.r[i], batch.sigma[i], batch.T[i], batch.q[i], batch.type[i], steps);
//                prices[i] = BinomialTree::priceParametersWorkspace(batch.S[i], batch.K[i], batch.r[i], batch.sigma[i], batch.T[i], batch.q[i], batch.type[i], steps, workspace);
        }
    }
    return prices;
}

std::vector<double> PricingDispatcher::priceBatchBlackScholesSIMD(const OptionBatch& batch){
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
std::vector<double> PricingDispatcher::priceBatchBinomialWorkspace(const std::vector<Option>& opts, int steps) {
    std::vector<double> prices(opts.size());
    #pragma omp parallel default(none) shared(opts, prices, steps)
    {
        BinomialWorkspace workspace(steps); // Thread-local
        #pragma omp for
        for (int i = 0; i < static_cast<int>(opts.size()); i++) {
            prices[i] = BinomialTree::priceWorkspace(opts[i], steps, workspace);
        }
    }
    return prices;
}

std::vector<GreekResult> PricingDispatcher::priceAndGreeks(const std::vector<Option>& opts, int steps) {
    std::vector<GreekResult> results(opts.size());
    #pragma omp parallel default(none) shared(opts, results, steps)
    {
        BinomialWorkspace workspace(steps); //once per thread; no matter if european or american, there
        #pragma omp for
        for (int i = 0; i < opts.size(); ++i) {
            const Option& opt = opts[i];
            GreekResult res;

            if (opt.style == OptionStyle::European) {
                res.price = BlackScholes::price(opt);
                res.greeks = BlackScholes::computeGreeks(opt);
            } else {
                res.price = BinomialTree::priceWorkspace(opt, steps, workspace);
                res.greeks = BinomialTree::computeGreeks(opt, workspace); // will reuse workspace; also using default parameters
            }

            results[i] = res;
        }
    }
    return results;
}

std::vector<Greeks> PricingDispatcher::greeks(const std::vector<Option>& opts, int steps) {
    std::vector<Greeks> results(opts.size());
    #pragma omp parallel default(none) shared(opts, results, steps)
    {
        BinomialWorkspace workspace(steps); //once per thread; no matter if european or american, there
        #pragma omp for
        for (int i = 0; i < opts.size(); ++i) {
            const Option& opt = opts[i];
            Greeks res;

            if (opt.style == OptionStyle::European) {
                res= BlackScholes::computeGreeks(opt);
            } else {
                res = BinomialTree::computeGreeks(opt, workspace); // will reuse workspace; also using default parameters
            }

            results[i] = res;
        }
    }
    return results;
}
