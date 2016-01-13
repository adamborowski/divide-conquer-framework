#include <bits/allocator.h>
#include <stdlib.h>
#include "RandomBucketAccessor.h"

template<class T>
template<typename... _Args>
RandomBucketAccessor<T>::RandomBucketAccessor(const int numThreads, const int threadsPerBucket, const int queueChangeFactor, _Args &... __args) :
        numThreads(numThreads),
        changeMaxCounter(queueChangeFactor),
        changeCounter(queueChangeFactor) {
    std::allocator<T> a;
    this->threadsPerBucket = threadsPerBucket == 0 ? numThreads : threadsPerBucket;
    numBuckets = (numThreads + this->threadsPerBucket - 1) / this->threadsPerBucket;
    buckets = a.allocate((unsigned long) numBuckets);
    for (int i = 0; i < numBuckets; i++) {
        a.construct(&buckets[i], std::forward<_Args>(__args)...);
    }
}

template<class T>
T *RandomBucketAccessor<T>::getInputBucket(int threadId) {
    return &buckets[threadId / threadsPerBucket];
}

template<class T>
T *RandomBucketAccessor<T>::getOutputBucket(int threadId) {
    if (numBuckets > 1) {
        changeCounter--;
        if (changeCounter == 0) {
            changeCounter = changeMaxCounter;
            return &buckets[getRandomBucketId()];
        }
    }
    return getInputBucket(threadId);
}

template<class T>
int RandomBucketAccessor<T>::getRandomBucketId() {
    return 0 + (rand() % (numBuckets - 1 - 0 + 1));
}

template<class T>
int RandomBucketAccessor<T>::getNumBuckets() {
    return numBuckets;
}

template<class T>
T *RandomBucketAccessor<T>::getBuckets() {
    return buckets;
}
