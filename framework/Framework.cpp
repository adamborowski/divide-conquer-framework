#include "Framework.h"
#include <omp.h>
#include <iostream>

using namespace std;

template<class TParam, class TResult>
TResult ProblemSolver<TParam, TResult>::process() {
    omp_set_num_threads(numThreads);
    TResult finalResult;
    bool work = true;
#pragma omp parallel shared(work, finalResult)
    {
        //todo shared variable = can we stop?
        //every thread iterates over common queue and processes each task
        cout << "\nThread" << omp_get_thread_num() << "started working";

        while (work) {
            Task<TParam, TResult> *task = taskContainer.pickFromQueue();

            if (task->isRootTask() and task->state == TaskState::COMPUTED) {
                //this is the root node = we just have the final result
                work = false;
                finalResult = task->result;
                break;
            }
            else if (task->state == TaskState::AWAITING) {
                //this node need's calculation or has to be divided
            }
            else if (task->state == TaskState::COMPUTED and task->brother.state==TaskState::COMPUTED){
                //we have two brothers ready to merge, put parent task to queue and mark it as CALCULATED
            }
        }

    }
    return finalResult;
}


template
class ProblemSolver<double, double>;

template<class TParams, class TResult>
Task<TParams, TResult> TaskContainer::createTask() {
    tasks.emplace_back(1);
    return tasks.back();
}

template<class TParams, class TResult>
void TaskContainer::putIntoQueue(Task<TParams, TResult> *task) {
#pragma omp critical (queue)
    {
        queue.push(task);
    }
}

template<class TParams, class TResult>
Task<TParams, TResult> *TaskContainer::pickFromQueue() {
#pragma omp critical (queue)
    {
        return queue.pop();
    }
}

bool Task::isRootTask() {
    return parent == nullptr;
}
