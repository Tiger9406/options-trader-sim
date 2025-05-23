cmake_minimum_required(VERSION 3.26)
project(options_simulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(options_simulator ../main.cpp
        src/EuropeanOption.h
        src/EuropeanOption.cpp)
