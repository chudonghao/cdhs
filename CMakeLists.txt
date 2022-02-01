cmake_minimum_required(VERSION 3.21)
project(cdhs)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED)
find_package(Boost REQUIRED)

set(TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

add_subdirectory(src)