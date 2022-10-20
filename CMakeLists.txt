cmake_minimum_required(VERSION 3.12)
project(MyProject VERSION 0.0.1)

add_library(
    hello
    hello.hpp
    hello.cpp
)

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
message(${OpenCV_INCLUDE_DIRS})

include_directories(${OpenCV_INCLUDE_DIRS})

set(SRC main.cpp)
add_executable(helloworld ${SRC})

target_link_libraries(helloworld ${OpenCV_LIBS} hello)
