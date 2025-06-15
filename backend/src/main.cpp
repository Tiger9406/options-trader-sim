//
// Created by xcao2 on 6/14/2025.
//

#include <string>
#include "api/ApiKeyLoader.h"

int main(){
    //use polygon api
    constexpr const char* host = "api.polygon.io";
    constexpr const char* port = "80";
    const std::string api_key = load_api_key();
    const std::string target = "/v3/snapshot/options/TSLA?apiKey=" + api_key;
    constexpr int version = 11; //http/1.1


    //use multithreading queue as inspired by hrt: multithreading in toucanserver
}