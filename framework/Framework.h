#ifndef DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H
#define DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H

#include <deque>
#include <string>
#include <queue>

using namespace std;


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


    TaskContainer(ProblemImpl<TParams, TResult> &problem) : problem(&problem) { };
};


template<class TParams, class TResult>
class AbstractProblemSolver {
protected:
    ProblemImpl<TParams, TResult> &problem;
public:
    bool debug;

    AbstractProblemSolver(ProblemImpl<TParams, TResult> &problem) : problem(problem) { }

    void output(std::string str);

    virtual TResult process(TParams params) = 0;
};

template<class TParams, class TResult>
class BaseProblemSolver : public AbstractProblemSolver<TParams, TResult> {
private:
    TaskContainer<TParams, TResult> taskContainer;
    int numThreads;
public:
    BaseProblemSolver(
            ProblemImpl<TParams, TResult> &problem,
            int numThreads
    ) :
            AbstractProblemSolver<TParams, TResult>(problem),
            taskContainer(problem),
            numThreads(numThreads) { }

    virtual TResult process(TParams params);
};

template<class TParams, class TResult>
class OptimizedProblemSolver : public AbstractProblemSolver<TParams, TResult> {
private:
    int numThreads;
    int threadsPerQueue;
    int parallelFactor;
public:
    OptimizedProblemSolver(
            ProblemImpl<TParams, TResult> &problem,
            int numThreads,
            int threadsPerQueue,
            int parallelFactor
    ) :
            AbstractProblemSolver<TParams, TResult>(problem),
            numThreads(numThreads),
            threadsPerQueue(threadsPerQueue),
            parallelFactor(parallelFactor) { }

    virtual TResult process(TParams params);
};


#endif //DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H
