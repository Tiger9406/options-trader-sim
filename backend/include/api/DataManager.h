#ifndef OPTIONS_SIMULATOR_DATA_MANAGER_H
#define OPTIONS_SIMULATOR_DATA_MANAGER_H


#include <unordered_map>
#include <string>
#include <mutex>
#include <vector>

#include "shared/Option.h"
#include "shared/OptionBatch.h"

class DataManager{
    OptionBatch batch_;
    std::unordered_map<std::string, size_t> instrument_to_idx_;
    std::mutex data_mutex_;

public:
    //register new option by name
    void register_chain(const std::vector<std::pair<std::string, Option>>& chain);
    
    // update internal batch from incoming json message
    void update_from_json(const std::string& json_msg);

    // return copy of batch for pricing engine; or should we return reference with mutex lock?
    OptionBatch get_batch_snapshot();
};

#endif //OPTIONS_SIMULATOR_DATA_MANAGER_H