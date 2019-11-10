cmake_minimum_required(VERSION 3.9)

project(fractal_explorer)

include_directories(include)

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -O3")

#set(SOURCES src/main.cpp src/point.cpp src/rectangle.cpp)
file(GLOB SOURCES "src/*.cpp")

add_executable(fractal_explorer ${SOURCES})
