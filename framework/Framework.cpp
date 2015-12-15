#include "Framework.h"
#include <omp.h>
#include <iostream>

using namespace std;

template<typename DataType, typename ResultType>
void ProblemSolver<DataType, ResultType>::start() {
    omp_set_num_threads(numThreads);
#pragma omp parallel
    {
        cout << "Thread" << omp_get_thread_num();
    }
}
