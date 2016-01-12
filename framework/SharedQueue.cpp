//
// Created by adam on 12.01.16.
//

#include "SharedQueue.h"

template<class T>
SharedQueue<T>::SharedQueue(long initialSize) : queue(initialSize) {

}

template<class T>
void SharedQueue<T>::putMany(T *source, const int count) {
    lock.lock();
    {
        for (int i = 0; i < count; i++) {
            queue.unsynchronized_push(source[i]);
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
            queue.unsynchronized_pop(destination[i]);
        }
    }
    lock.unlock();

}

template<class T>
void SharedQueue<T>::put(T item) {
    lock.lock();
    {
        queue.unsynchronized_push(item);
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
        queue.unsynchronized_pop(*item);
        lock.unlock();
        return true;
    }
}
