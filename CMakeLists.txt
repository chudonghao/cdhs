cmake_minimum_required(VERSION 3.21)
project(cdhs)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED)

add_subdirectory(src)