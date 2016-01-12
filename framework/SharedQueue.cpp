//
// Created by adam on 12.01.16.
//

#include "SharedQueue.h"

template<class T>
SharedQueue<T>::SharedQueue(long initialSize)
#ifndef USE_STL_QUEUE
: queue(initialSize)
#endif
{

}

template<class T>
void SharedQueue<T>::putMany(T *source, const int count) {
    lock.lock();
    {
        for (int i = 0; i < count; i++) {
#ifdef USE_STL_QUEUE
            queue.push(source[i]);
#else
            queue.unsynchronized_push(source[i]);
#endif
        }
    }
    lock.unlock();
}

template<class T>
void SharedQueue<T>::pickMany(T *destination, const int count, int &numPicked) {
    lock.lock();
    {
        numPicked = count;
        for (int i = 0; i < count; i++) {
            if (queue.empty()) {
                numPicked = i;
                break;
            }
#ifdef USE_STL_QUEUE
            destination[i] = queue.pop();
#else
            queue.unsynchronized_pop(destination[i]);
#endif
        }
    }
    lock.unlock();

}

template<class T>
void SharedQueue<T>::put(T item) {
    lock.lock();
    {
#ifdef USE_STL_QUEUE
        queue.push(item);
#else
        queue.unsynchronized_push(item);
#endif
    }
    lock.unlock();
}

template<class T>
bool SharedQueue<T>::pick(T *item) {
    lock.lock();
    if (queue.empty()) {
        lock.unlock();
        return false;
    }
    else {
#ifdef USE_STL_QUEUE
        *item = queue.front();
        queue.pop();
#else
        queue.unsynchronized_pop(*item);
#endif
        lock.unlock();
        return true;
    }
}
