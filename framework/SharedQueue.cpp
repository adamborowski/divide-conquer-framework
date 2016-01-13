//
// Created by adam on 12.01.16.
//

#include "SharedQueue.h"


template<class T>
SharedQueue<T>::SharedQueue(long initialSize)
#if USE_BOOST_QUEUE || USE_CUSTOM_QUEUE
        : queue(initialSize)
#endif
{

}

template<class T>
void SharedQueue<T>::putMany(T *source, const int count) {
    lock.lock();
    {
        for (int i = 0; i < count; i++) {
#if USE_STL_QUEUE
            queue.push(source[i]);
#elif USE_BOOST_QUEUE
            queue.unsynchronized_push(source[i]);
#elif USE_CUSTOM_QUEUE
            queue.put(source[i]);
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
#if USE_STL_QUEUE
            destination[i] = queue.pop();
#elif USE_BOOST_QUEUE
            queue.unsynchronized_pop(destination[i]);
#elif USE_CUSTOM_QUEUE
            queue.pop(&destination[i]);
#endif
        }
    }
    lock.unlock();

}

template<class T>
void SharedQueue<T>::put(T item) {
    lock.lock();
    {
#if USE_STL_QUEUE
        queue.push(item);
#elif USE_BOOST_QUEUE
        queue.unsynchronized_push(item);
#elif USE_CUSTOM_QUEUE
        queue.put(item);
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
#if USE_STL_QUEUE
        *item = queue.front();
        queue.pop();
#elif USE_BOOST_QUEUE
        queue.unsynchronized_pop(*item);
#elif USE_CUSTOM_QUEUE
        queue.pop(item);
#endif
        lock.unlock();
        return true;
    }
}
