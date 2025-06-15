#Performance Test Executable
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(performance_test ${TEST_SOURCES})
target_include_directories(performance_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shared
        ${CMAKE_CURRENT_SOURCE_DIR}/include/legacy
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/shared
        ${CMAKE_SOURCE_DIR}/include/legacy
)
target_link_libraries(performance_test PRIVATE core)