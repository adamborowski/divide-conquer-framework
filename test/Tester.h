//
// Created by adam on 02.01.16.
//

#ifndef DIVIDE_CONQUER_FRAMEWORK_TESTER_H
#define DIVIDE_CONQUER_FRAMEWORK_TESTER_H


#include "../framework/Framework.h"

class Tester {
public:
    void run(const int numRepeats);

private:
    int numThreadsValues[100] = {1, 2, 4, 8, 16, 32, 64, 128, 244};
    int threadsPerQueueValues[100] = {1, 2, 4, 8, 16, 32, 64, 128, 244};
    int parallelFactorValues[100] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};

};


#endif //DIVIDE_CONQUER_FRAMEWORK_TESTER_H
