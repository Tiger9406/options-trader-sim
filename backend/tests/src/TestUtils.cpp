//
// Created by xcao2 on 6/14/2025.
//
#include "TestUtils.h"
#include <vector>
#include <cmath>
#include <iomanip>
#include <stdio.h>

void summarizePricingErrors(const std::vector<double>& prices1,
                            const std::vector<double>& prices2,
                            const std::vector<Option>& options) {
    size_t n = prices1.size();
    if (n != prices2.size() || n != options.size()) {
        std::cerr << "Error: Vector sizes must match.\n";
        return;
    }

    struct FullErrorInfo {
        size_t index;
        Option opt;
        double price1, price2, absErr, relErr;
    };

    const double MIN_DENOMINATOR = 1e-2;
    double absErrorSum = 0.0, relErrorSum = 0.0;
    double maxAbsErr = 0.0, maxRelErr = 0.0;
    int countRelErrOver5pct = 0, skippedRelErr = 0;

    std::vector<FullErrorInfo> fullErrors;

    for (size_t i = 0; i < n; ++i) {
        double bin = prices1[i];
        double baw = prices2[i];
        double absErr = std::abs(bin - baw);

        absErrorSum += absErr;
        if (absErr > maxAbsErr) maxAbsErr = absErr;

        if (std::abs(bin) >= MIN_DENOMINATOR) {
            double relErr = absErr / std::abs(bin);
            relErrorSum += relErr;
            if (relErr > maxRelErr) maxRelErr = relErr;
            if (relErr > 0.05) ++countRelErrOver5pct;

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
    std::cout << "Count > 5% Rel Error:     " << countRelErrOver5pct << " options\n";


    // Sort and display top offenders
    std::sort(fullErrors.begin(), fullErrors.end(), [](const auto& a, const auto& b) {
        return a.relErr > b.relErr;
    });

    std::cout << "\nWorst Offenders (Top 10 by Relative Error):\n";
    std::cout << "Idx  Type   S        K        r       q       sig     T        Model1   Model2   AbsErr     RelErr%\n";
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
                  << std::setw(8)  << e.price1    << " "
                  << std::setw(8)  << e.price2    << " "
                  << std::setw(8)  << e.absErr    << " "
                  << std::setw(12) << e.relErr * 100 << "%\n";
    }
}