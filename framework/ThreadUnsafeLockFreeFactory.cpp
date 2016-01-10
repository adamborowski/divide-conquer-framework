#include "ThreadUnsafeLockFreeFactory.h"

template<class T>
void ThreadUnsafeLockFreeFactory<T>::resetChunk() {
    chunkCounter = 0;
    chunk = new T[chunkSize];
}

template<class T>
ThreadUnsafeLockFreeFactory<T>::ThreadUnsafeLockFreeFactory(int chunkSize) : chunkSize(chunkSize) {
    resetChunk();
}

template<class T>
T *ThreadUnsafeLockFreeFactory<T>::create() {
    if (chunkCounter == chunkSize) {
        resetChunk();
    }
    T *newObject = &chunk[chunkCounter];
    chunkCounter++;
    return newObject;
}
