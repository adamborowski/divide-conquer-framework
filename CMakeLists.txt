cmake_minimum_required(VERSION 3.3)
project(divide_conquer_framework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Framework.h)
add_executable(divide_conquer_framework ${SOURCE_FILES})