#ifndef DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H
#define DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H

#include <deque>
#include <string>
#include <queue>
#include <omp.h>
#include "ThreadStats.h"
#include "OmpLock.h"

using namespace std;


template<class TParams>
struct DividedParams {
public:
    TParams param1;
    TParams param2;
};

enum class TaskState {
    AWAITING, COMPUTED, DONE, DEAD //DEAD is also DONE and we are sure that it not exists int the queue
};

template<class TParams, class TResult>
class Task {
public:
    TParams params;
    TResult result;
    TaskState state;
    Task<TParams, TResult> *parent = nullptr; // if null and state==done - finish program
    Task<TParams, TResult> *brother = nullptr;

    bool isRootTask = false;

    OmpLock computations;
};


template<class TParams, class TResult>
class ProblemImpl {
public:
    virtual bool testDivide(TParams param) = 0;

    virtual TResult merge(TResult a, TResult b) = 0;

    virtual TResult compute(TParams param) = 0;

    virtual DividedParams<TParams> divide(TParams a) = 0;

    virtual std::string toString(TParams params) = 0;

    virtual std::string toString(TResult result) = 0;
};


template<class TParams, class TResult>
class TaskContainer {
private:
    std::deque<Task<TParams, TResult>> tasks;
    std::queue<Task<TParams, TResult> *> queue;
    ProblemImpl<TParams, TResult> *problem;
public:


    Task<TParams, TResult> *createTask();

    void putIntoQueue(Task<TParams, TResult> *task);

    Task<TParams, TResult> *pickFromQueue();

    unsigned long getQueueSize() {
        return queue.size();
    }

    TaskContainer(ProblemImpl<TParams, TResult> &problem) : problem(&problem) { };
};


template<class TParams, class TResult>
class AbstractProblemSolver {
protected:
    ProblemImpl<TParams, TResult> &problem;
    int numThreads;
    ThreadStats threadStats;
public:
    bool debug;

    AbstractProblemSolver(ProblemImpl<TParams, TResult> &problem, int numThreads) : problem(problem),
                                                                                    numThreads(numThreads),
                                                                                    threadStats(numThreads) { }

    void output(std::string str);

    ThreadStats &getThreadStats();

    virtual TResult process(TParams params) = 0;
};

template<class TParams, class TResult>
class BaseProblemSolver : public AbstractProblemSolver<TParams, TResult> {
private:
    TaskContainer<TParams, TResult> taskContainer;

public:
    BaseProblemSolver(
            ProblemImpl<TParams, TResult> &problem,
            int numThreads
    ) :
            AbstractProblemSolver<TParams, TResult>(problem, numThreads),
            taskContainer(problem) { }

    virtual TResult process(TParams params);
};

template<class TParams, class TResult>
class OptimizedProblemSolver : public AbstractProblemSolver<TParams, TResult> {
private:
    int numThreads;
    int threadsPerQueue;
    int parallelFactor;
    int chunkSize;
    TaskContainer<TParams, TResult> taskContainer;
public:
    OptimizedProblemSolver(
            ProblemImpl<TParams, TResult> &problem,
            int numThreads,
            int threadsPerQueue,
            int parallelFactor,
            int chunkSize
    ) :
            AbstractProblemSolver<TParams, TResult>(problem, numThreads),
            numThreads(numThreads),
            threadsPerQueue(threadsPerQueue),
            taskContainer(problem),
            parallelFactor(parallelFactor),
            chunkSize(chunkSize) { }

    virtual TResult process(TParams params);
};


#endif //DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H
