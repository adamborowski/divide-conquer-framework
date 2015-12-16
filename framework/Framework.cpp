#include "Framework.h"
#include <omp.h>
#include <iostream>

using namespace std;

template
class ProblemSolver<double, double>;

template<class TParam, class TResult>
TResult ProblemSolver<TParam, TResult>::process() {
    omp_set_num_threads(numThreads);
    TResult finalResult;
    bool work = true;
#pragma omp parallel shared(work, finalResult)
    {
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
                if (problem.testDivide(task->params)) {
                    // divide task into smaller tasks and push to queue
                    Task<TParam, TResult> *task1 = taskContainer.createTask();
                    Task<TParam, TResult> *task2 = taskContainer.createTask();

                    task1->state = TaskState::AWAITING;
                    task2->state = TaskState::AWAITING;

                    task1->parent = task;
                    task1->brother = task2;
                    task2->brother = task1;

                    const DividedParams<TParam> &dividedParams = problem.divide(task->params);
                    task1->params = dividedParams.param1;
                    task2->params = dividedParams.param2;

                    taskContainer.putIntoQueue(task1);
                    taskContainer.putIntoQueue(task2);
                }
                else {
                    task->result = problem.compute(task->params); // calculate directly the result
                    task->state = TaskState::COMPUTED; // change the state to "ready to merge"
                    taskContainer.putIntoQueue(task); // put task to be merged later
                }
            }
            else if (task->state == TaskState::COMPUTED and task->brother->state == TaskState::COMPUTED) {
                //we have two brothers ready to merge, put parent task to queue and mark it as CALCULATED
            }
        }

    }
    return finalResult;
}

template<class TParams, class TResult>
Task<TParams, TResult> *TaskContainer<TParams,TResult>::createTask() {
    tasks.emplace_back();
    return &tasks.back();
}

template<class TParams, class TResult>
void TaskContainer<TParams,TResult>::putIntoQueue(Task<TParams, TResult> *task) {
#pragma omp critical (queue)
    {
        queue.push(task);
    }
}

template<class TParams, class TResult>
Task<TParams, TResult> *TaskContainer<TParams,TResult>::pickFromQueue() {
    Task<TParams, TResult> *pTask;
#pragma omp critical (queue)
    {
        pTask = queue.pop();
    }
    return pTask;
}
template<class TParams, class TResult>
bool Task<TParams,TResult>::isRootTask() {
    return parent == nullptr;
}
