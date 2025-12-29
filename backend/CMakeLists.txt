cmake_minimum_required(VERSION 3.22)

project(options_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    # Check common Homebrew paths if OpenMP wasn't found
    if(NOT OpenMP_ROOT)
        if(EXISTS "/opt/homebrew/opt/libomp")
            set(OpenMP_ROOT "/opt/homebrew/opt/libomp")
        elseif(EXISTS "/usr/local/opt/libomp")
            set(OpenMP_ROOT "/usr/local/opt/libomp")
        endif()
    endif()
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
    cmake_policy(SET CMP0144 NEW)
    cmake_policy(SET CMP0167 NEW)
endif()

# enable optimization flags
add_compile_options(-O3 -march=native -Wall -Wextra -pedantic)

# enable OpenMP & optimization flags for multi-core
find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/StaticMain.cpp")

#create library
add_library(core STATIC ${SOURCES})

target_include_directories(core PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(core
        PUBLIC Boost::headers
        PUBLIC OpenMP::OpenMP_CXX
        PUBLIC OpenSSL::SSL
        PUBLIC OpenSSL::Crypto
        PUBLIC pthread
)

# Main Executable
add_executable(options_simulator src/main.cpp)
target_link_libraries(options_simulator PRIVATE core)

add_subdirectory(tests)