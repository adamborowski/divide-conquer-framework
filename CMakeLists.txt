cmake_minimum_required(VERSION 3.3)
project(divide_conquer_framework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

if (APPLE)
    include_directories(/usr/local/Cellar/libiomp/20150701/include/libiomp)
endif ()

set(SOURCE_FILES main.cpp framework/Framework.cpp framework/ProblemSolver.cpp test/Timer.cpp test/Timer.h test/Tester.cpp test/Tester.h)
add_executable(divide_conquer_framework ${SOURCE_FILES})
