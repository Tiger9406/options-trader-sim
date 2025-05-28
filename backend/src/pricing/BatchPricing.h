//
// Created by xcao2 on 5/27/2025.
//

#ifndef OPTIONS_SIMULATOR_BATCHPRICING_H
#define OPTIONS_SIMULATOR_BATCHPRICING_H

#include <cmath>
#include <vector>
#include <algorithm>
#include "../shared/Option.h"
#include "OptionBatch.h"
#include "BlackScholes.h"

// black-scholes assuming batch is all european
inline std::vector<double> blackScholesBatch(const OptionBatch& batch) {
    size_t N = batch.size();
    std::vector<double> prices(N);

    double S, K, r, sigma, T, d1, d2;
    size_t i;
    #pragma omp parallel for simd default(none) \
        private(i, S, K, r, sigma, T, d1, d2) shared(batch, prices, N)
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

#endif //OPTIONS_SIMULATOR_BATCHPRICING_H
