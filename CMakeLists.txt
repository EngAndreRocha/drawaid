cmake_minimum_required(VERSION 3.12)
project(MyProject VERSION 0.0.1)

add_library(
    hello
    hello.hpp
    hello.cpp
)

set(SRC main.cpp)
add_executable(helloworld ${SRC})

target_link_libraries(helloworld hello)
