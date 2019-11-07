cmake_minimum_required(VERSION 3.9)

project(TESTING)

include_directories(include)

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -Wextra")

set(SOURCES src/main.cpp src/point.cpp src/rectangle.cpp)
#file(GLOB SOURCES "src/*.cpp")

add_executable(TESTING ${SOURCES})
