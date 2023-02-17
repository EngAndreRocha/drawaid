cmake_minimum_required(VERSION 3.12)
project(DRAWAID VERSION 0.0.1.1  HOMEPAGE_URL "https://github.com/EngAndreRocha/graphite-drawing-helper" DESCRIPTION "Retrieves every .jpg from either the input folder in this current directory or the path provided and, if it has a human face, creates one folder for each photo with several images to help you draw an acurate portrait of a person. If no path or filename is given, the program will either use or create a folder named drawaid_input_folder for your convinience and use in the working directory of the program." LANGUAGES CXX)

#set_property (TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
#target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

add_library(
    ipl
    ipl.hpp
    ipl.cpp
)

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_VERSION: ${OpenCV_VERSION}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
message(STATUS "OpenCV_DIR = ${OpenCV_DIR}")

include_directories(${OpenCV_INCLUDE_DIRS})

set(SRC main.cpp)
add_executable(drawaid ${SRC})

target_compile_features(drawaid PUBLIC cxx_std_17)
target_compile_features(ipl PUBLIC cxx_std_17)
target_link_libraries(drawaid ${OpenCV_LIBS} ipl)
