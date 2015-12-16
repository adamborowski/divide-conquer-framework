#ifndef DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H
#define DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H

#include <deque>
#include <string>

#include "BlockingQueue.h"


template<class TParams>
struct DividedParams {
public:
    TParams param1;
    TParams param2;
};

enum class TaskState {
    AWAITING, COMPUTED, DONE
};

template<class TParams, class TResult>
class Task {
public:
    TParams params;
    TResult result;
    TaskState state;
    Task<TParams, TResult> *parent; // if null and state==done - finish program
    Task<TParams, TResult> *brother;

    inline bool isRootTask();
};


template<class TParams, class TResult>
class ProblemImpl {
public:
    virtual bool testDivide(TParams param) = 0;

    virtual TResult merge(TResult a, TResult b) = 0;

    virtual TResult compute(TParams param) = 0;

    virtual DividedParams<TParams> divide(TParams a) = 0;
};


template<class TParams, class TResult>
class TaskContainer {
private:
    std::deque<Task<TParams, TResult>> tasks;
    BlockingQueue<Task<TParams, TResult> *> queue;
public:


    Task<TParams, TResult> *createTask();

    void putIntoQueue(Task<TParams, TResult> *task);

    Task<TParams, TResult> *pickFromQueue();


};

template<class TParams, class TResult>
class ProblemSolver {
private:
    ProblemImpl<TParams, TResult> &problem;
    TaskContainer<TParams, TResult> taskContainer;

    int numThreads;
    bool debug;
protected:
    void output(std::string str);

public:
    ProblemSolver(ProblemImpl<TParams, TResult> &problem, int numThreads, bool debug) : problem(problem),
                                                                                        taskContainer(),
                                                                                        numThreads(numThreads),
                                                                                        debug(debug) { }

    TResult process(TParams params);
};

#endif //DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H
