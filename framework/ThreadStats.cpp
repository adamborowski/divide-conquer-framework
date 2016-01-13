#include "ThreadStats.h"
#include <cmath>

void ThreadStats::tick(int threadId) {
    values[threadId]++;
}

void ThreadStats::tickMany(int threadId, int howMany) {
    values[threadId] += howMany;
}

void ThreadStats::calculate() {
    double sum = 0;
    for (int i = 0; i < numThreads; i++) {
        sum += values[i];
    }
    average = sum / numThreads;
    sum = 0;
    for (int i = 0; i < numThreads; i++) {
        sum += (values[i] - average) * (values[i] - average);
    }
    stdDeviation = sqrt(sum / (numThreads - 1));

}

double ThreadStats::getStdDeviation() {
    return stdDeviation;
}

double ThreadStats::getAverage() {
    return average;
}

ThreadStats::ThreadStats(int numThreads) {
    values = new double[numThreads];
    for (int i = 0; i < numThreads; i++) {
        values[i] = 0;
    }
    this->numThreads = numThreads;
}

ThreadStats::~ThreadStats() {
    delete values;
}