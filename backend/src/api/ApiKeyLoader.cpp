//
// Created by xcao2 on 6/15/2025.
//

#include "api/ApiKeyLoader.h"
#include <fstream>
#include <stdexcept>

std::string load_api_key(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open API key file");
    }
    std::string key;
    std::getline(file, key);
    return key;
}
