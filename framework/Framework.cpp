#include "Framework.h"

using namespace std;


template<class TParams, class TResult>
Task<TParams, TResult> *TaskContainer<TParams, TResult>::createTask() {
    Task<TParams, TResult> *pTask;
#pragma omp critical (createTask)
    {
        tasks.emplace_back();
        pTask = &tasks.back();
        pTask->computations.lock();
    }
    return pTask;
}

template<class TParams, class TResult>
void TaskContainer<TParams, TResult>::putIntoQueue(Task<TParams, TResult> *task) {
#pragma omp critical (queue)
    {
        queue.push(task);
        task->computations.unlock();
//        cout << "\n@" << omp_get_thread_num() << ": put into queue: " << problem->toString(task->params) <<
//        " after: " << queue.size() << "";
    }
}

template<class TParams, class TResult>
Task<TParams, TResult> *TaskContainer<TParams, TResult>::pickFromQueue() {
    Task<TParams, TResult> *result = nullptr;
#pragma omp critical (queue)
    {

        if (!queue.empty()) {
            result = queue.front();
            queue.pop(); // todo lockuj się na tasku ale najpierw zwolnij dostęp do kolejki
            result->computations.lock();
//            cout << "\n@" << omp_get_thread_num() << ": picked from queue: " << problem->toString(result->params) <<
//            " after: " << queue.size() << "";


        }
    }
    return result;
}

template<class TParams, class TResult>
ThreadStats &AbstractProblemSolver<TParams, TResult>::getThreadStats() {
    return this->threadStats;
}
