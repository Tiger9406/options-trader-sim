#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include "MarketDataFeed.h"
#include "OptionBatch.h"
#include "PricingDispatcher.h"
#include "BenchmarkUtils.h"

int main() {
    constexpr size_t NUM_EUROPEAN = 500000;
    constexpr size_t NUM_American = 500000;

    constexpr int tickIntervalMs = 100;
    constexpr double dt = 1.0 / 365.0;

    OptionBatch batch;
    batch.reserve(NUM_American+NUM_EUROPEAN);

    // random option batch through testing util
    //should just move the 24 bytes control block
    std::vector<Option> options = generateMixedOptions(NUM_EUROPEAN, NUM_American);
    batch = toBatch(options);

    // asio prep
    boost::asio::io_context io;

    //random fluctuation to price & volatility
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> noise(0.0, 0.01);

    // define call back per tick
    auto onTick = [&]() {
        for (size_t i = 0; i < batch.size(); ++i) {
            batch.S[i] += batch.S[i] * noise(gen);
            batch.sigma[i] = std::max(0.01, batch.sigma[i] + 0.01 * noise(gen));
            batch.T[i] = std::max(1e-6, batch.T[i] - dt);
        }
        auto prices = PricingDispatcher::priceBatch(batch, 1000);
        std::cout << "Tick: " << prices[0] << " ... " << prices[prices.size() - 1] << std::endl;
    };

    // start market feed
    MarketDataFeed feed(io, onTick, tickIntervalMs);
    feed.start();

    // run
    io.run();

    return 0;
}
