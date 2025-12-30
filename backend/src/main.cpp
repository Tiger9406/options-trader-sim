//
// Created by xcao2 on 6/14/2025.
//

#include <string>
#include "api/ApiKeyLoader.h"
#include "api/Client.h"

#include "api/DataManager.h"
#include "pricing/PricingDispatcher.h"
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>

int main(){
    // constexpr const char* host = "api.polygon.io";
    // constexpr const char* port = "80";
    // const std::string api_key = load_api_key();
    // const std::string target = "/v3/snapshot/options/TSLA?apiKey=" + api_key;
    // constexpr int version = 11; //http/1.1
    try{
        auto ioc = std::make_shared<net::io_context>();
        auto ctx = std::make_shared<ssl::context>(ssl::context::tlsv12_client);
        
        // data manager; shared pointer for client
        auto data_manager = std::make_shared<DataManager>();

        PricingDispatcher engine;

        Option btc_call;
        btc_call.K = 90000.0;
        btc_call.T = 0.5;        // 6 months
        btc_call.r = 0.05;       // 5% rate
        btc_call.type = Call;     // From your enum
        btc_call.style = European; // From your enum
        btc_call.S = 0.0;

        data_manager->register_chain({{"BTC-2JAN26-90000-C", btc_call}});

        // create client to connect to deribit
        auto client = std::make_shared<Client>(*ioc, *ctx);
        
        // capture data_manager by value (increasing ref count)
        client->set_callback([data_manager](std::string msg) {
            data_manager->update_from_json(std::move(msg));
        });

        client->run("test.deribit.com", "443", "BTC-2JAN26-90000-C");

        std::thread pricing_thread([&](){
            while (true){
                // std::cout<< "Calculating Prices...\n";
                OptionBatch curr_batch = data_manager->get_batch_snapshot();
                if(curr_batch.size() > 0){
                    std::vector<double> results = engine.priceBatch(curr_batch);
                    std::cout<< "Latest Price for [0]: " << results[0] << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
        ioc->run();
        pricing_thread.join();
    } catch (const std::exception& e){
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}