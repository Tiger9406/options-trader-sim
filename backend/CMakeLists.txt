cmake_minimum_required(VERSION 3.22)

project(options_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-O3 -march=native -Wall -Wextra -pedantic)

##enable OpenMP & optimization flags for multi-core
find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/legacy/*.cpp
)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/include/*.h
        ${CMAKE_SOURCE_DIR}/include/legacy/*.h
)

#create library
add_library(core STATIC ${SOURCES})
target_include_directories(core PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/shared
        ${CMAKE_SOURCE_DIR}/include/legacy
)
target_link_libraries(core
        PUBLIC Boost::system
        PUBLIC OpenMP::OpenMP_CXX
        PUBLIC pthread
)

# Main Executable
add_executable(options_simulator StaticMain.cpp)
target_link_libraries(options_simulator PRIVATE core)

add_subdirectory(tests)