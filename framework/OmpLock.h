//
// Created by adam on 09.01.16.
//

#ifndef DIVIDE_CONQUER_FRAMEWORK_OMPLOCK_H
#define DIVIDE_CONQUER_FRAMEWORK_OMPLOCK_H

#include <omp.h>

class OmpLock {
    omp_lock_t _lock;
    int owner = false;
public:
    OmpLock();

    ~OmpLock();

    bool isLocked();
    void lock();

    void unlock();
};


#endif //DIVIDE_CONQUER_FRAMEWORK_OMPLOCK_H
