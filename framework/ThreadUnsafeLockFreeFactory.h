//
// Created by adam on 10.01.16.
// based on http://www.boost.org/doc/libs/1_59_0/doc/html/lockfree/rationale.html#lockfree.rationale.memory_management
//

#ifndef DIVIDE_CONQUER_FRAMEWORK_THREADUNSAFELOCKFREEPOOL_H
#define DIVIDE_CONQUER_FRAMEWORK_THREADUNSAFELOCKFREEPOOL_H
/**
 * This factory will never free memory to prevent touch locking OS freeing
 */
template<class T>
class ThreadUnsafeLockFreeFactory {
private:
    T *chunk;
    int chunkCounter;
    int creationCounter;
    int chunkSize;
    void resetChunk();
public:
    ThreadUnsafeLockFreeFactory(long chunkSize);
    T* create();
    int getNumCreatedElements();
};


#endif //DIVIDE_CONQUER_FRAMEWORK_THREADUNSAFELOCKFREEPOOL_H
