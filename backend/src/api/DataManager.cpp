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

void debug_print_json(simdjson::ondemand::value val, int indent = 0) {
    std::string spacer(indent, ' ');
    
    switch (val.type()) {
        case simdjson::ondemand::json_type::array: {
            std::cout << "[\n";
            for (auto element : val.get_array()) {
                std::cout << spacer << "  ";
                debug_print_json(element.value(), indent + 2);
                std::cout << ",\n";
            }
            std::cout << spacer << "]";
            break;
        }
        case simdjson::ondemand::json_type::object: {
            std::cout << "{\n";
            for (auto field : val.get_object()) {
                std::cout << spacer << "  \"" << field.unescaped_key() << "\": ";
                debug_print_json(field.value(), indent + 2);
                std::cout << ",\n";
            }
            std::cout << spacer << "}";
            break;
        }
        case simdjson::ondemand::json_type::number:
            std::cout << double(val);
            break;
        case simdjson::ondemand::json_type::string:
            std::cout << "\"" << std::string_view(val) << "\"";
            break;
        case simdjson::ondemand::json_type::boolean:
            std::cout << (bool(val) ? "true" : "false");
            break;
        case simdjson::ondemand::json_type::null:
            std::cout << "null";
            break;
    }
}


/*
{
    "jsonrpc": "2.0",
    "method": "subscription",
    "params": {
        "channel": "ticker.BTC-2JAN26-90000-C.100ms",
        "data": {
        "timestamp": 1.76702e+12,
        "state": "open",
        "stats": {
            "high": 0.014,
            "low": 0.0029,
            "price_change": 75,
            "volume": 1273.7,
            "volume_usd": 793751,
        },
        "greeks": {
            "theta": -149.082,
            "delta": 0.25353,
            "gamma": 9e-05,
            "vega": 28.107,
            "rho": 2.17889,
        },
        "settlement_price": 0.00827248,
        "index_price": 87573.6,
        "instrument_name": "BTC-2JAN26-90000-C",
        "last_price": 0.014,
        "min_price": 0.0001,
        "max_price": 0.0365,
        "open_interest": 2559.6,
        "mark_price": 0.0058,
        "best_ask_price": 0.006,
        "best_bid_price": 0.0055,
        "interest_rate": 0,
        "mark_iv": 38.86,
        "bid_iv": 37.92,
        "ask_iv": 39.48,
        "underlying_price": 87638.9,
        "underlying_index": "BTC-2JAN26",
        "estimated_delivery_price": 87573.6,
        "best_ask_amount": 100,
        "best_bid_amount": 100,
        },
    },
}
*/
void DataManager::update_from_json(const std::string& raw_json) {
    // thread_local to not re allocate the parser every time a packet arrives
    thread_local ondemand::parser parser;
    auto json = padded_string(raw_json);

    try {
        ondemand::document doc = parser.iterate(json);

        // std::cout << "\nIncoming Message \n";
        // debug_print_json(doc.get_value());
        // std::cout << "\n------------------\n";
        
        auto params = doc.find_field("params");
        if (params.error()) return;
        auto data = params.find_field("data");
        if (data.error()) return;
        
        std::string_view instrument;
        data["instrument_name"].get(instrument);

        double spot = data["underlying_price"];
        double iv = data["mark_iv"];
        double mark = data["mark_price"];

        {
            // locks the mutex; in this local scope created by {}; out of scope, destructed, unlocks
            std::lock_guard<std::mutex> lock(data_mutex_);
            auto it = instrument_to_idx_.find(std::string(instrument));
            if (it != instrument_to_idx_.end()) {
                size_t idx = it->second;
                // update live strick price
                batch_.S[idx] = spot; 
                batch_.sigma[idx] = iv / 100.0;
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