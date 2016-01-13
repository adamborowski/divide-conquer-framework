//
// Created by adam on 09.01.16.
//

#ifndef DIVIDE_CONQUER_FRAMEWORK_THREADSTATS_H
#define DIVIDE_CONQUER_FRAMEWORK_THREADSTATS_H

/**
 * Average and Std deviation
 */
class ThreadStats {
    long *values;
    int numThreads;
    double average;
    double stdDeviation;
public:
    void tick(int threadId);
    void tickMany(int threadId, int howMany);

    void calculate();

    double getAverage();

    double getStdDeviation();

    long getThreadTicks(int threadId);

    ThreadStats(int numThreads);

    ~ThreadStats();
};

#endif //DIVIDE_CONQUER_FRAMEWORK_THREADSTATS_H
