cmake_minimum_required(VERSION 2.8)
project(divide_conquer_framework)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -mmic")

SET(CMAKE_CXX_COMPILER "icpc")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -w")
SET(CMAKE_CXX_FLAGS_DEBUG "-g")







set(SOURCE_FILES main.cpp framework/Framework.cpp framework/AbstractProblemSolver.cpp framework/BaseProblemSolver.cpp test/Timer.cpp test/Timer.h framework/ThreadStats.cpp framework/ThreadStats.cpp framework/ThreadStats.h framework/OmpLock.cpp framework/OmpLock.h framework/ThreadUnsafeLockFreeFactory.cpp framework/ThreadUnsafeLockFreeFactory.h framework/SharedQueue.cpp framework/SharedQueue.h framework/ThreadUnsafeLockFreeQueue.cpp framework/ThreadUnsafeLockFreeQueue.h framework/RandomBucketAccessor.cpp framework/RandomBucketAccessor.h)
add_executable(divide_conquer_framework ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(divide_conquer_framework)
