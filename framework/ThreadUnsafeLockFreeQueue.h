//
// Created by adam on 12.01.16.
//

#ifndef DIVIDE_CONQUER_FRAMEWORK_THREADUNSAFELOCKFREEQUEUE_H
#define DIVIDE_CONQUER_FRAMEWORK_THREADUNSAFELOCKFREEQUEUE_H

#include "ThreadUnsafeLockFreeFactory.h"
#include "ThreadUnsafeLockFreeFactory.cpp"


template<class T>
struct Node {
    T value;
    Node<T> *next;
};

template<class T>
class ThreadUnsafeLockFreeQueue {
private:
    ThreadUnsafeLockFreeFactory<Node<T>> factory;
    Node<T> *first;
    Node<T> *last;
    Node<T> *trash;
public:
    ThreadUnsafeLockFreeQueue(long initialSize);

    void put(T);

    bool empty();

    void pop(T *where);

    void debugPrint();
};


#endif //DIVIDE_CONQUER_FRAMEWORK_THREADUNSAFELOCKFREEQUEUE_H
