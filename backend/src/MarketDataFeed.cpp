#include "MarketDataFeed.h"
#include <numeric>
#include <iostream>

MarketDataFeed::MarketDataFeed(boost::asio::io_context& io_, Callback onTick_, int interval_ms_)
        : timer(io_),
          onTick(std::move(onTick_)),
          interval_ms(interval_ms_) {}

void MarketDataFeed::start() {
    tick();
}

void MarketDataFeed::stop() {
    timer.cancel();
}

void MarketDataFeed::tick() {
    auto start = std::chrono::high_resolution_clock::now();
    onTick();
    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Tick took " << ms << " ms\n";
    timer.expires_after(std::chrono::milliseconds(interval_ms));
    timer.async_wait([this](const boost::system::error_code&) {
        tick();
    });
}
