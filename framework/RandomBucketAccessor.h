#ifndef DIVIDE_CONQUER_FRAMEWORK_RANDOMBUCKETACCESSOR_H
#define DIVIDE_CONQUER_FRAMEWORK_RANDOMBUCKETACCESSOR_H


template<class T>
class RandomBucketAccessor {
private:
    int numThreads;
    int threadsPerBucket;
    int numBuckets;
    T *buckets;

    int getRandomBucketId();

    int changeMaxCounter;
    int changeCounter;

public:
    template<typename... _Args>
    RandomBucketAccessor(const int numThreads, const int threadsPerBucket, const int queueChangeFactor, _Args &... __args);

    T *getInputBucket(int threadId); // bucket where I belong to

    T *getOutputBucket(int threadId); // maybe my bucket but sometimes another...
    T *getBuckets();

    int getNumBuckets();

};

#endif //DIVIDE_CONQUER_FRAMEWORK_RANDOMBUCKETACCESSOR_H
