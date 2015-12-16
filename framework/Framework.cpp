#include "Framework.h"
#include <omp.h>
#include <iostream>
#include <unistd.h>

using namespace std;


template<class TParams, class TResult>
TResult ProblemSolver<TParams, TResult>::process(TParams params) {
    omp_set_num_threads(numThreads);
    TResult finalResult;
    bool work = true;

    // create initial task
    Task<TParams, TResult> *rootTask = taskContainer.createTask();
    rootTask->parent = nullptr;
    rootTask->brother = nullptr;
    rootTask->isRootTask = true;
    rootTask->params = params;
    rootTask->state = TaskState::AWAITING;
    taskContainer.putIntoQueue(rootTask);

#pragma omp parallel shared(work, finalResult)
    {
        //every thread iterates over common queue and processes each task
        output("started working");

        while (work) {
            //fixme: only for demonstration purposes - ensure proper log order
            usleep(10000); // only for debug purposes
            {
                Task<TParams, TResult> *task;

                task = taskContainer.pickFromQueue();


                if (task == nullptr) {
                    output("skip, no task!");
                    continue;
                }
                if (task->isRootTask) {
                    output("got root task!");
                }

                string common =
                        "got task: (" + to_string(task->params.a) + " " + to_string(task->params.b) + ") / ";

//                output(common);

                if (task->isRootTask and task->state == TaskState::COMPUTED) {
                    output(common + "root task = " + to_string(task->result));

                    //this is the root node = we just have the final result
                    work = false;
                    finalResult = task->result;
                    //push dummy task to unlock the queue
                    task->state = TaskState::DONE;
                    taskContainer.putIntoQueue(task);
                }
                else if (task->state == TaskState::AWAITING) {

                    //this node need's calculation or has to be divided
                    if (problem.testDivide(task->params)) {
                        output(common + "divide");
                        // divide task into smaller tasks and push to queue
                        Task<TParams, TResult> *task1 = taskContainer.createTask();
                        Task<TParams, TResult> *task2 = taskContainer.createTask();

                        task1->state = TaskState::AWAITING;
                        task2->state = TaskState::AWAITING;

                        task1->parent = task;
                        task1->brother = task2;
                        task2->brother = task1;

                        const DividedParams<TParams> &dividedParams = problem.divide(task->params);
                        task1->params = dividedParams.param1;
                        task2->params = dividedParams.param2;

                        taskContainer.putIntoQueue(task1);
                        taskContainer.putIntoQueue(task2);
                    }
                    else {
                        task->result = problem.compute(task->params); // calculate directly the result
                        task->state = TaskState::COMPUTED; // change the state to "ready to merge"
                        output(common + "compute = ~" + to_string(task->result));
                        taskContainer.putIntoQueue(task); // put task to be merged later

                    }
                }
                else if (task->parent != nullptr and task->state == TaskState::COMPUTED and
                         task->brother->state == TaskState::COMPUTED) {

                    //we have two brothers ready to merge, put parent task to queue and mark it as CALCULATED
                    Task<TParams, TResult> *parent = task->parent;
                    output(common + "parent = " + (parent == nullptr ? "nullptr" : "instance"));
                    parent->result = problem.merge(task->result, task->brother->result);
                    output(common + "merge brothers ~" + to_string(task->result) + " and ~" +
                           to_string(task->brother->result) + " = " + to_string(parent->result));
                    parent->state = TaskState::COMPUTED;
                    task->state = TaskState::DONE;
                    task->brother->state = TaskState::DONE;
                    //put parent into queue again
                    taskContainer.putIntoQueue(parent);
                }
                else {
                    //todo investigate why this task must be re-enqueued
                    taskContainer.putIntoQueue(task);
                }
            }
        }

    }
    return finalResult;
}

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


template<class TParams, class TResult>
void ProblemSolver<TParams, TResult>::output(std::string str) {
#pragma omp critical (output)
    {
        cout << "\n#" << omp_get_thread_num() << ": " << str;
    }
}

