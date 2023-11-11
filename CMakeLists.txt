cmake_minimum_required(VERSION 3.26)
project(simple_matching_engine)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(simple_matching_engine
        main.cpp
        Limit.h
        Orderbook.h
        Limit.cpp
        Orderbook.cpp
        Side.h
        Order.h
)
