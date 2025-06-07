#include "DispatcherBenchmarks.h"
#include "BinomialTree.h"
#include "BAW.h"
#include "BenchmarkUtils.h"
#include <cmath>
#include <iomanip>


void summarizePricingErrors(const std::vector<double>& binomialPrices,
                            const std::vector<double>& BAWPrices,
                            const std::vector<Option>& options) {
    size_t n = binomialPrices.size();
    if (n != BAWPrices.size() || n != options.size()) {
        std::cerr << "Error: Vector sizes must match.\n";
        return;
    }

    struct FullErrorInfo {
        size_t index;
        Option opt;
        double bin, baw, absErr, relErr;
    };

    const double MIN_DENOMINATOR = 1e-2;
    double absErrorSum = 0.0, relErrorSum = 0.0;
    double maxAbsErr = 0.0, maxRelErr = 0.0;
    int countRelErrOver1pct = 0, skippedRelErr = 0;

    std::vector<FullErrorInfo> fullErrors;

    for (size_t i = 0; i < n; ++i) {
        double bin = binomialPrices[i];
        double baw = BAWPrices[i];
        double absErr = std::abs(bin - baw);

        absErrorSum += absErr;
        if (absErr > maxAbsErr) maxAbsErr = absErr;

        if (std::abs(bin) >= MIN_DENOMINATOR) {
            double relErr = absErr / std::abs(bin);
            relErrorSum += relErr;
            if (relErr > maxRelErr) maxRelErr = relErr;
            if (relErr > 0.01) ++countRelErrOver1pct;

            fullErrors.push_back({i, options[i], bin, baw, absErr, relErr});
        } else {
            ++skippedRelErr;
        }
    }

    double meanAbsErr = absErrorSum / n;
    double meanRelErr = (n - skippedRelErr > 0) ? (relErrorSum / (n - skippedRelErr)) : 0.0;

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Pricing Error Summary:\n";
    std::cout << "-----------------------\n";
    std::cout << "Total Options:            " << n << '\n';
    std::cout << "Mean Absolute Error:      " << meanAbsErr << '\n';
    std::cout << "Mean Relative Error:      " << meanRelErr * 100 << "%\n";
    std::cout << "Max Absolute Error:       " << maxAbsErr << '\n';
    std::cout << "Max Relative Error:       " << maxRelErr * 100 << "%\n";
    std::cout << "Count > 1% Rel Error:     " << countRelErrOver1pct << " options\n";
    std::cout << "Skipped Rel Errors:       " << skippedRelErr << " (binomial price too small)\n";

    // Sort and display top offenders
    std::sort(fullErrors.begin(), fullErrors.end(), [](const auto& a, const auto& b) {
        return a.relErr > b.relErr;
    });

    std::cout << "\nWorst Offenders (Top 10 by Relative Error):\n";
    std::cout << "Idx  Type   S        K        r       q       sig     T       Binom     BAW       AbsErr     RelErr%\n";
    std::cout << "---- ------ -------- -------- ------- ------- ------- -------- -------- -------- ------------\n";

    for (size_t i = 0; i < std::min<size_t>(10, fullErrors.size()); ++i) {
        const auto& e = fullErrors[i];
        const char* typeStr = (e.opt.type == Call ? "Call" : "Put");

        std::cout << std::setw(4)  << e.index << "  "
                  << std::setw(6)  << typeStr << " "
                  << std::setw(8)  << std::fixed << std::setprecision(4) << e.opt.S     << " "
                  << std::setw(8)  << e.opt.K     << " "
                  << std::setw(7)  << e.opt.r     << " "
                  << std::setw(7)  << e.opt.q     << " "
                  << std::setw(7)  << e.opt.sigma << " "
                  << std::setw(7)  << e.opt.T     << " "
                  << std::setw(8)  << e.bin       << " "
                  << std::setw(8)  << e.baw       << " "
                  << std::setw(8)  << e.absErr    << " "
                  << std::setw(12) << e.relErr * 100 << "%\n";
    }
}

int main() {
    constexpr int NUM_EUROPEAN = 1'000'000;
    constexpr int NUM_AMERICAN = 10'000;

//    benchmarkDispatcherSeparateStyle(NUM_EUROPEAN, NUM_AMERICAN);
//    benchmarkDispatcherMixedStyle(NUM_EUROPEAN, NUM_AMERICAN);
    std::vector<Option> allOptions = generateMixedOptions(0, NUM_AMERICAN);
    std::vector<double> binomialPrices = benchmarkParallelization(&BinomialTree::price, 0, NUM_AMERICAN, allOptions);
    std::vector<double> BAWPrices = benchmarkParallelization(&BAW::price, 0, NUM_AMERICAN, allOptions);
    std::vector<double> blackScholesPrice = benchmarkParallelization(&BAW::price, NUM_EUROPEAN, 0);

    summarizePricingErrors(binomialPrices, BAWPrices, allOptions);
    return 0;
}