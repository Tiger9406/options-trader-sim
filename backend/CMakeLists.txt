cmake_minimum_required(VERSION 3.22)
project(options_simulator)

set(CMAKE_CXX_STANDARD 17)

##enable OpenMP & optimization flags for multi-core
find_package(OpenMP REQUIRED)
add_compile_options(-O3 -march=native -fopenmp)

file(GLOB CORE_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/legacy/*.cpp
)
add_library(core STATIC ${CORE_SOURCES})

target_include_directories(core PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/shared
        ${CMAKE_SOURCE_DIR}/include/legacy
)

# Main Executable
add_executable(options_simulator main.cpp)
target_link_libraries(options_simulator
        PRIVATE core
        PRIVATE OpenMP::OpenMP_CXX
)

#Performance Test Executable
add_executable(performance_test
        tests/performance_test.cpp
        tests/DispatcherBenchmarks.cpp
        tests/legacy/TemplateBenchmarks.cpp
)
target_link_libraries(performance_test
        PRIVATE core
        PRIVATE OpenMP::OpenMP_CXX
)