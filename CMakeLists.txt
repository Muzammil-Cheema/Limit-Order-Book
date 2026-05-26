cmake_minimum_required(VERSION 4.3)

set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

project(LIMIT_ORDER_BOOK LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")

add_executable(engine src/main.cpp)
# target_include_directories()