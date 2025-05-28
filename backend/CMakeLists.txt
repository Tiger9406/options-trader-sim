cmake_minimum_required(VERSION 3.26)
project(options_simulator)

set(CMAKE_CXX_STANDARD 17)

##enable OpenMP & optimization flags for multi-core
find_package(OpenMP REQUIRED)
add_compile_options(-O3 -march=native)

# Static Library: core
add_library(core STATIC
        src/legacy/EuropeanOption.cpp
)

# Main Executable
add_executable(options_simulator main.cpp)

target_include_directories(options_simulator PRIVATE
        src
        src/shared
        src/legacy
        src/pricing
)

target_link_libraries(options_simulator
        PRIVATE core
        PRIVATE OpenMP::OpenMP_CXX
)


#Performance Test Executable
add_executable(performance_test
        tests/performance_test.cpp
)

target_include_directories(performance_test PRIVATE
        src
        src/shared
        src/legacy
        src/pricing
)

target_link_libraries(performance_test
        PRIVATE core
        PRIVATE OpenMP::OpenMP_CXX
)