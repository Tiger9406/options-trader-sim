//
// Created by xcao2 on 5/27/2025.
//

#ifndef OPTIONS_SIMULATOR_OPTIONBATCH_H
#define OPTIONS_SIMULATOR_OPTIONBATCH_H

#include <vector>
#include "Option.h"


struct OptionBatch{
    std::vector<double> S, K, r, sigma, T, q;
    std::vector<OptionType> type;
    std::vector<OptionStyle> style;
    //structure of arrays SoA; easier manipulation w/ SIMD
    void reserve(const size_t N){
        S.reserve(N); K.reserve(N); r.reserve(N);
        sigma.reserve(N); T.reserve(N); q.reserve(N); type.reserve(N);
        style.reserve(N);
    }
    void push_back(const Option& opt){
        S.push_back(opt.S);
        K.push_back(opt.K);
        r.push_back(opt.r);
        sigma.push_back(opt.sigma);
        T.push_back(opt.T);
        q.push_back(opt.q);
        type.push_back(opt.type);
        style.push_back(opt.style);
    }
    size_t size() const {
        return S.size();
    }
};

// Convert AoS -> SoA; only for European
inline OptionBatch toBatch(const std::vector<Option>& options) {
    OptionBatch batch;
    batch.reserve(options.size());

    for (const auto& opt : options) {
            batch.push_back(opt);
    }
    return batch;
}

#endif //OPTIONS_SIMULATOR_OPTIONBATCH_H
