#include "Framework.h"
using namespace std;



template<class TParams, class TResult>
Task<TParams, TResult> *TaskContainer<TParams, TResult>::createTask() {
    Task<TParams, TResult> *pTask;
#pragma omp critical (createTask)
    {
        tasks.emplace_back();
        pTask = &tasks.back();
    }
    return pTask;
}

template<class TParams, class TResult>
void TaskContainer<TParams, TResult>::putIntoQueue(Task<TParams, TResult> *task) {
#pragma omp critical (queue_put)
    {
        queue.push(task);
//        cout << "\n@" << omp_get_thread_num() << ": put into queue: " << problem->toString(task->params) <<
//        " after: " << queue.size() << "";
    }
}

template<class TParams, class TResult>
Task<TParams, TResult> *TaskContainer<TParams, TResult>::pickFromQueue() {
    Task<TParams, TResult> *result = nullptr;
#pragma omp critical (queue_pick)
    {

        if (!queue.empty()) {
            result = queue.front();
            queue.pop();
//            cout << "\n@" << omp_get_thread_num() << ": picked from queue: " << problem->toString(result->params) <<
//            " after: " << queue.size() << "";


        }
    }
    return result;
}

template<class TParams, class TResult>
bool TaskContainer<TParams, TResult>::hasTask() {
    return !queue.empty();
}
