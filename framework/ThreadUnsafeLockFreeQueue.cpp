//
// Created by adam on 12.01.16.
//

#include <iostream>
#include "ThreadUnsafeLockFreeQueue.h"

template<class T>
ThreadUnsafeLockFreeQueue<T>::ThreadUnsafeLockFreeQueue(long initialSize) : factory(initialSize), first(nullptr), trash(nullptr), last(nullptr), count(0) {

}

template<class T>
void ThreadUnsafeLockFreeQueue<T>::put(T t) {
    Node<T> *newLast;
    if (trash == nullptr) {
        newLast = factory.create();
    }
    else {
        //we can re-use trash
        newLast = trash; //let's use first trash
        trash = newLast->next; // maybe trash had next item??
    }

    //we have newLast allocated, let's initialize
    newLast->value = t;
    newLast->next = nullptr;
    //put item as last (and first, if we have empty queue)
    if (last != nullptr) {
        last->next = newLast;
    }
    last = newLast;
    if (first == nullptr) { // we put to empty queue
        first = last;
    }
    count++;
}

template<class T>
bool ThreadUnsafeLockFreeQueue<T>::empty() {
    return count == 0;
}

template<class T>
void ThreadUnsafeLockFreeQueue<T>::pop(T *where) {
    *where = first->value; //send pop value
    Node<T> *newFront = first->next; // our front will move to next value behind current front
    first->next = trash; // join old front and old trash
    trash = first; // and move front to trash
    first = newFront;
    if (first == nullptr) {
        last = nullptr;
    }
    count--;
}

template<class T>
void ThreadUnsafeLockFreeQueue<T>::debugPrint() {
    Node<T> *st = first;
    std::cout << std::endl;
    while (st != nullptr) {
        std::cout << " " << st->value;
        st = st->next;
    }
    st = trash;
    std::cout << " / ";
    while (st != nullptr) {
        std::cout << " " << st->value;
        st = st->next;
    }
    std::cout.flush();
}

template<class T>
int ThreadUnsafeLockFreeQueue<T>::getCount() {
    return count;
}
