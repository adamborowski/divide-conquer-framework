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
    Task<TParams, TResult> *parent = nullptr; // if null and state==done - finish program
    Task<TParams, TResult> *brother = nullptr;

    bool isRootTask = false;
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

    bool hasTask();


    TaskContainer(ProblemImpl<TParams, TResult> &problem) : problem(&problem) { };
};

template<class TParams, class TResult>
class ProblemSolver {
private:
    ProblemImpl<TParams, TResult> &problem;
    TaskContainer<TParams, TResult> taskContainer;

    int numThreads;
    bool debug;
public:
    void output(std::string str);

public:
    ProblemSolver(ProblemImpl<TParams, TResult> &problem, int numThreads, bool debug) : problem(problem),
                                                                                        taskContainer(problem),
                                                                                        numThreads(numThreads),
                                                                                        debug(debug) { }

    TResult process(TParams params);
};

#endif //DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H
