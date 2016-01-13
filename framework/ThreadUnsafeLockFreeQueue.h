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
    int count;
    int putCount;
    int popCount;
public:
    ThreadUnsafeLockFreeQueue(long initialSize);

    void put(T);

    bool empty();

    void pop(T *where);

    void debugPrint();

    int getCount();
    int getPutCount();
    int getPopCount();
};


#endif //DIVIDE_CONQUER_FRAMEWORK_THREADUNSAFELOCKFREEQUEUE_H
