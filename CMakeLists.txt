cmake_minimum_required(VERSION 3.25)

#set(CMAKE_CXX_COMPILER "")

project(Limit_Order_Book LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")
set(CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

add_executable(matching_engine
        src/main.cpp
        src/order.cpp
        src/trade.cpp
        src/order_book.cpp
)

target_include_directories(matching_engine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")