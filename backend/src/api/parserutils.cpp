#include "simdjson.h"
#include "shared/OptionBatch.h"
#include <iostream>

using namespace simdjson;

void parse_deribit_ticker(const std::string& raw_msg, 
                          std::unordered_map<std::string, size_t>& mapping, 
                          OptionBatch& batch) {
    static ondemand::parser parser;
    
    // make padded string
    auto json = padded_string(raw_msg);
    
    try {
        ondemand::document doc = parser.iterate(json);
        auto data = doc["params"]["data"];
        
        // extract necessary fields
        std::string_view inst_name = data["instrument_name"];
        double last_price = data["last_price"];
        
        // lookup in mapping & update batch
        auto it = mapping.find(std::string(inst_name));
        if (it != mapping.end()) {
            size_t idx = it->second;
            batch.S[idx] = last_price;
        }
    } catch (const simdjson_error& e) {
        std::cerr << "Simdjson Error: " << e.what() << std::endl;
    }
}