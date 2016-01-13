#ifndef DIVIDE_CONQUER_FRAMEWORK_SHAREDQUEUE_H
#define DIVIDE_CONQUER_FRAMEWORK_SHAREDQUEUE_H

#include "OmpLock.h"
#include "ThreadUnsafeLockFreeQueue.h"
#include <boost/lockfree/queue.hpp>
#include <queue>

#define STL_QUEUE 1
#define BOOST_QUEUE 2
#define CUSTOM_QUEUE 3


#define ________CHOICE CUSTOM_QUEUE


#define USE_STL_QUEUE ________CHOICE == STL_QUEUE
#define USE_BOOST_QUEUE ________CHOICE == BOOST_QUEUE
#define USE_CUSTOM_QUEUE ________CHOICE == CUSTOM_QUEUE

/**
 * Queue shared across some threads.
 * We can't user #pragma omp critical, because it forces global scope of lock, for every queue instance
 * TOTO: is it worth to specify initial queue size as an argument?
 */
template<class T>
class SharedQueue {
private:
    OmpLock lock;
#if USE_STL_QUEUE
    std::queue<T> queue;
#elif USE_BOOST_QUEUE
    boost::lockfree::queue<T> queue;
#elif USE_CUSTOM_QUEUE
    ThreadUnsafeLockFreeQueue<T> queue;
#endif
public:
    SharedQueue(long initialSize);

    void put(T item);

    bool pick(T *item);

    void putMany(T *source, const int count);

    void pickMany(T *destination, const int count, int &numPicked);

    int getCountNotSynchronized();
    int getPutCountNotSynchronized();
    int getPopCountNotSynchronized();
};


#endif //DIVIDE_CONQUER_FRAMEWORK_SHAREDQUEUE_H
