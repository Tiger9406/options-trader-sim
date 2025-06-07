//
// Created by xcao2 on 6/1/2025.
//

#ifndef OPTIONS_SIMULATOR_BENCHMARKUTILS_H
#define OPTIONS_SIMULATOR_BENCHMARKUTILS_H

#include <string>
#include <iostream>
#include <vector>
#include <chrono>

template<typename Func>
inline double benchmark(const std::string& label, Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    double result = func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << label << " took " << duration.count() << " ms\n";
    return duration.count();
}

template<typename Container, typename Func>
inline double applyAndSum(const Container& container, Func func) {
    double sum = 0.0;
    for (const auto& item : container) sum += func(item);
    return sum;
}

#include <vector>
#include <random>
#include <tuple>
#include <algorithm>
#include "shared/Option.h"

class RandomGenerator {
public:
    std::mt19937 gen;
    std::uniform_real_distribution<double> spot{90, 110};
    std::uniform_real_distribution<double> strike{90, 110};
    std::uniform_real_distribution<double> rate{0.01, 0.05};
    std::uniform_real_distribution<double> vol{0.05, 0.3};
    std::uniform_real_distribution<double> expiry{0.2, 2.0};
    std::uniform_real_distribution<double> div_yield{0.0, 0.04};
    std::bernoulli_distribution callPut{0.5};

    RandomGenerator() : gen(std::random_device{}()) {}

    auto generateOptionParams() {
        return std::make_tuple(
                spot(gen),
                strike(gen),
                rate(gen),
                vol(gen),
                expiry(gen),
                div_yield(gen),
                callPut(gen) ? Call : Put
        );
    }
};

inline std::vector<Option> generateOptions(int count, OptionStyle style) {
    RandomGenerator rng;
    std::vector<Option> options;
    options.reserve(count);
    for (int i = 0; i < count; i++) {
        auto [S, K, r, sigma, T, q, type] = rng.generateOptionParams();
        options.emplace_back(S, K, r, sigma, T, q, type, style);
    }
    return options;
}

inline std::vector<Option> generateMixedOptions(int european, int american) {
    RandomGenerator rng;
    std::vector<Option> options;
    options.reserve(european + american);

    std::vector<OptionStyle> styles;
    styles.insert(styles.end(), european, OptionStyle::European);
    styles.insert(styles.end(), american, OptionStyle::American);
    std::shuffle(styles.begin(), styles.end(), rng.gen);

    for (OptionStyle style : styles) {
        auto [S, K, r, sigma, T, q, type] = rng.generateOptionParams();
        options.emplace_back(S, K, r, sigma, T, q, type, style);
    }

    return options;
}


#endif //OPTIONS_SIMULATOR_BENCHMARKUTILS_H
