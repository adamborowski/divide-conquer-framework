#include "Framework.h"
#include <omp.h>
#include <unistd.h>
#include <iostream>

using namespace std;

template<class TParams, class TResult>
void AbstractProblemSolver<TParams, TResult>::output(std::string str) {
    if (debug) {
#pragma omp critical (output)
        {
            cout << "\n#" << omp_get_thread_num() << ": " << str;
        }
    }
}
