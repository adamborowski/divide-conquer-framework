//
// Created by adam on 12.01.16.
//

#ifndef DIVIDE_CONQUER_FRAMEWORK_SHAREDQUEUE_H
#define DIVIDE_CONQUER_FRAMEWORK_SHAREDQUEUE_H

#include "OmpLock.h"
#include <boost/lockfree/queue.hpp>

/**
 * Queue shared across some threads.
 * We can't user #pragma omp critical, because it forces global scope of lock, for every queue instance
 * TOTO: is it worth to specify initial queue size as an argument?
 */
template <class T>
class SharedQueue {
private:
    OmpLock lock;
    boost::lockfree::queue<T> queue;
public:
    SharedQueue(long initialSize);
    void put(T item);
    bool pick(T *item);
    void putMany(T *source, const int count);
    void pickMany(T *destination, const int count, int &numPicked);
};


#endif //DIVIDE_CONQUER_FRAMEWORK_SHAREDQUEUE_H
