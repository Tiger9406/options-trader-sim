//
// Created by xcao2 on 6/14/2025.
//

#include <string>
#include "api/ApiKeyLoader.h"
#include "api/Client.h"

#include "api/DataManager.h"

int main(){
    // constexpr const char* host = "api.polygon.io";
    // constexpr const char* port = "80";
    // const std::string api_key = load_api_key();
    // const std::string target = "/v3/snapshot/options/TSLA?apiKey=" + api_key;
    // constexpr int version = 11; //http/1.1

    auto ioc = std::make_shared<net::io_context>();
    auto ctx = std::make_shared<ssl::context>(ssl::context::tlsv12_client);
    
    // data manager; shared pointer for client
    auto data_manager = std::make_shared<DataManager>();

    // create client to connect to deribit
    auto client = std::make_shared<Client>(*ioc, *ctx);
    
    // capture data_manager by value (increasing ref count)
    client->set_callback([data_manager](std::string msg) {
        data_manager->update_from_json(std::move(msg));
    });

    client->run("www.deribit.com", "443", "BTC-2JAN26-90000-C");
    ioc->run();
}