//
// Created by xcao2 on 6/1/2025.
//

#ifndef OPTIONS_SIMULATOR_MARKETDATAFEED_H
#define OPTIONS_SIMULATOR_MARKETDATAFEED_H

#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <random>


class MarketDataFeed {
public:
    using Callback = std::function<void()>;

    MarketDataFeed(boost::asio::io_context& io, Callback onTick, int interval_ms);
    void start();
    void stop();

private:
    void tick();

    boost::asio::steady_timer timer;
    Callback onTick;
    int interval_ms;
};


#endif //OPTIONS_SIMULATOR_MARKETDATAFEED_H
