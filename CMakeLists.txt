cmake_minimum_required(VERSION 3.25)
project(Limit_Order_Book LANGUAGES CXX)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)



set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")
set(LOB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LOB_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(LOB_TEST_DIR "${CMAKE_SOURCE_DIR}/tests")



add_library(core
        ${LOB_SOURCE_DIR}/order.cpp
        ${LOB_SOURCE_DIR}/trade.cpp
        ${LOB_SOURCE_DIR}/order_book.cpp
)

target_include_directories(core PUBLIC ${LOB_INCLUDE_DIR})



add_executable(matching_engine ${LOB_SOURCE_DIR}/main.cpp)

target_link_libraries(matching_engine PRIVATE core)



add_executable(test_matching_engine
        ${LOB_TEST_DIR}/order_tests.cpp
        ${LOB_TEST_DIR}/trade_tests.cpp
)

target_link_libraries(test_matching_engine PRIVATE
        core
        GTest::gtest_main
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(test_matching_engine)
