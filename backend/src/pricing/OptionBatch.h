//
// Created by xcao2 on 5/27/2025.
//

#ifndef OPTIONS_SIMULATOR_OPTIONBATCH_H
#define OPTIONS_SIMULATOR_OPTIONBATCH_H

#include <vector>
#include "../shared/Option.h"


struct OptionBatch{
    std::vector<double> S, K, r, sigma, T;
    std::vector<OptionType> type;
    //structure of arrays SoA; easier manipulation w/ SIMD
    void reserve(const size_t N){
        S.reserve(N); K.reserve(N); r.reserve(N);
        sigma.reserve(N); T.reserve(N); type.reserve(N);
    }
    void push_back(const Option& opt){
        S.push_back(opt.S);
        K.push_back(opt.K);
        r.push_back(opt.r);
        sigma.push_back(opt.sigma);
        T.push_back(opt.T);
        type.push_back(opt.type);
    }
    size_t size() const {
        return S.size();
    }
};

// Convert AoS -> SoA; only for European
inline OptionBatch toEuropeanBatch(const std::vector<Option>& options) {
    OptionBatch batch;
    batch.reserve(options.size());

    for (const auto& opt : options) {
        if (opt.style == OptionStyle::European)
            batch.push_back(opt);
    }
    return batch;
}

#endif //OPTIONS_SIMULATOR_OPTIONBATCH_H
