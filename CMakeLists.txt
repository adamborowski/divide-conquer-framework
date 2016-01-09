cmake_minimum_required(VERSION 3.3)
project(divide_conquer_framework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

if (APPLE)
    include_directories(/usr/local/Cellar/libiomp/20150701/include/libiomp)
endif ()


FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp framework/Framework.cpp framework/AbstractProblemSolver.cpp framework/BaseProblemSolver.cpp test/Timer.cpp test/Timer.h framework/ThreadStats.cpp framework/ThreadStats.cpp framework/ThreadStats.h framework/OmpLock.cpp framework/OmpLock.h)
add_executable(divide_conquer_framework ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(divide_conquer_framework ${Boost_LIBRARIES})
