#include "Framework.h"
#include <omp.h>
#include <iostream>


using namespace std;

template<class TParams, class TResult>
TResult OptimizedProblemSolver<TParams, TResult>::process(TParams params) {
    omp_set_num_threads(this->numThreads);
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
    ThreadStats &threadStats = this->threadStats;


#pragma omp parallel shared(work, finalResult, threadStats)
    {
        const int threadId = omp_get_thread_num();

        //every thread iterates over common queue and processes each task
        this->output("started working");
//        cout << "\nnum threads: " + to_string(omp_get_num_threads()) + " / " + to_string(numThreads);

        while (work) {
            Task<TParams, TResult> *task;

            task = taskContainer.pickFromQueue();


            if (task == nullptr) {
                this->output("skip, no task!");
                continue;
            }
            else {
                threadStats.tick(threadId);
            }
            if (task->isRootTask) {
                this->output("got root task!");
            }

            string common =
                    "got task: (" + to_string(task->params.a) + " " + to_string(task->params.b) + ") / ";

//                output(common);

            if (task->isRootTask and task->state == TaskState::COMPUTED) {
                this->output(common + "root task = " + to_string(task->result));

                //this is the root node = we just have the final result
                work = false;
                finalResult = task->result;
                //push dummy task to unlock the queue
                task->state = TaskState::DONE;
                taskContainer.putIntoQueue(task);
            }
            else if (task->state == TaskState::AWAITING) {

                //this node need's calculation or has to be divided
                if (this->problem.testDivide(task->params)) {
                    this->output(common + "divide");
                    // divide task into smaller tasks and push to queue
                    Task<TParams, TResult> *task1 = taskContainer.createTask();
                    Task<TParams, TResult> *task2 = taskContainer.createTask();

                    task1->state = TaskState::AWAITING;
                    task2->state = TaskState::AWAITING;

                    task1->parent = task;
                    task1->brother = task2;
                    task2->brother = task1;

                    const DividedParams<TParams> &dividedParams = this->problem.divide(task->params);
                    task1->params = dividedParams.param1;
                    task2->params = dividedParams.param2;

                    taskContainer.putIntoQueue(task1);
                    taskContainer.putIntoQueue(task2);
                }
                else {
                    task->result = this->problem.compute(task->params); // calculate directly the result
                    task->state = TaskState::COMPUTED; // change the state to "ready to merge"
                    this->output(common + "compute = ~" + to_string(task->result));
                    taskContainer.putIntoQueue(task); // put task to be merged later

                }
            }
            else if (task->parent != nullptr and task->state == TaskState::COMPUTED and
                     task->brother->state == TaskState::COMPUTED) {

                //we have two brothers ready to merge, put parent task to queue and mark it as CALCULATED
                Task<TParams, TResult> *parent = task->parent;
                this->output(common + "parent = " + (parent == nullptr ? "nullptr" : "instance"));
                parent->result = this->problem.merge(task->result, task->brother->result);
                this->output(common + "merge brothers ~" + to_string(task->result) + " and ~" +
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
    return finalResult;
}