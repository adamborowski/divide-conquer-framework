#include "ThreadUnsafeLockFreeFactory.h"

template<class T>
void ThreadUnsafeLockFreeFactory<T>::resetChunk() {
    chunkCounter = 0;
    chunk = new T[chunkSize];
}

template<class T>
ThreadUnsafeLockFreeFactory<T>::ThreadUnsafeLockFreeFactory(long chunkSize) : chunkSize(chunkSize), chunkCounter(0), creationCounter(0) {
}

template<class T>
T *ThreadUnsafeLockFreeFactory<T>::create() {
    if (chunkCounter == 0) {
        resetChunk();
    }
    T *newObject = &chunk[chunkCounter];
    chunkCounter++;
    creationCounter++;
    if (chunkCounter == chunkSize) {
        chunkCounter = 0;
    }
    return newObject;
}
template<class T>
int ThreadUnsafeLockFreeFactory<T>::getNumCreatedElements() {
    return creationCounter;
}
