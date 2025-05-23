cmake_minimum_required(VERSION 3.26)
project(options_simulator)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        src
        src/shared
        src/european
        src/option
)

# define static library for later development
add_library(core STATIC
        src/european/EuropeanOption.cpp
)

# execute for testing
add_executable(options_simulator main.cpp)

#link
target_link_libraries(options_simulator core)