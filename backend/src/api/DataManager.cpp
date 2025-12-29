#include "api/DataManager.h"
#include "api/simdjson.h"
#include <iostream>

using namespace simdjson;

void DataManager::register_chain(const std::vector<std::pair<std::string, Option>>& chain) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    batch_.reserve(chain.size());
    for (size_t i = 0; i < chain.size(); ++i) {
        const auto& [name, opt] = chain[i];
        batch_.push_back(opt);
        instrument_to_idx_[name] = i;
    }
}



void DataManager::update_from_json(const std::string& raw_json) {
    // thread_local to not re allocate the parser every time a packet arrives
    thread_local ondemand::parser parser;
    auto json = padded_string(raw_json);

    try {
        ondemand::document doc = parser.iterate(json);
        
        // data's in params->data, extract
        auto data = doc["params"]["data"];
        
        std::string_view instrument = data["instrument_name"];
        double mark_price = data["mark_price"]; // gotta check documentation for field names

        {
            // locks the mutex; in this local scope created by {}; out of scope, destructed, unlocks
            std::lock_guard<std::mutex> lock(data_mutex_);
            auto it = instrument_to_idx_.find(std::string(instrument));
            if (it != instrument_to_idx_.end()) {
                size_t idx = it->second;
                // update live strick price
                batch_.S[idx] = mark_price; 
            }
        }
    } catch (const std::exception& e) {
        // ignore msgs that errors out
        std::cerr << "DataManager JSON Parse Error: " << e.what() << std::endl;
    }
}

OptionBatch DataManager::get_batch_snapshot() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    return batch_; // for atomic copy
}