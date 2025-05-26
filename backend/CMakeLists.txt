cmake_minimum_required(VERSION 3.26)
project(options_simulator)

set(CMAKE_CXX_STANDARD 17)

# define static library for later development
add_library(core STATIC
        src/european/EuropeanOption.cpp
)

# execute for testing
add_executable(options_simulator main.cpp)

target_include_directories(options_simulator PRIVATE
        src
        src/shared
        src/european
        src/american
        src/option
        src/pricing
)

#link
target_link_libraries(options_simulator core)

add_executable(performance_test
        tests/performance_test.cpp
)

target_include_directories(performance_test PRIVATE
        src
        src/shared
        src/european
        src/american
        src/option
        src/pricing
)

target_link_libraries(performance_test core) # Needed if still using EuropeanOptionInheritance
