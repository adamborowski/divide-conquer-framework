#include "Framework.h"
#include <omp.h>
#include <iostream>
#include <unistd.h>


using namespace std;

template<class TParams, class TResult>
TResult BaseProblemSolver<TParams, TResult>::process(TParams params) {
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


#pragma omp parallel
    {
        const int threadId = omp_get_thread_num();

        //every thread iterates over common queue and processes each task
        this->output("started working");

        while (work) {
            Task<TParams, TResult> *task;

            task = taskContainer.pickFromQueue();


            if (task == nullptr) {
                this->output("skip, no task!");
                usleep(100000);
                continue;
            }
            else {
                threadStats.tick(threadId);
            }
            if (task->state == TaskState::DONE || task->state == TaskState::DEAD) {
                //we assume that is the second node from merge
                task->state = TaskState::DEAD;
                continue;
            }
            if (task->isRootTask) {
                this->output("got root task!");
            }
            string common =
                    "got task: (" + to_string(task->params.a) + " " + to_string(task->params.b) + ") / ";

            if (task->isRootTask and task->state == TaskState::COMPUTED) {
                this->output(common + "root task = " + to_string(task->result));

                //this is the root node = we just have the final result
                work = false;
                finalResult = task->result;
                task->state = TaskState::DEAD;
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
                    task2->parent = task;
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
                task->state = TaskState::DEAD; // we know that task doesn't exist in queue so it will be always dead
                task->brother->state = TaskState::DONE; // we don't know if brother is in queue
                //put parent into queue again
                taskContainer.putIntoQueue(parent);
            }
            else if (task->parent != nullptr and task->state == TaskState::COMPUTED and
                     task->brother->state == TaskState::AWAITING) {
                //sytuacja, w której nie mamy jeszcze drugiego brata gotowego, zatem nic nie róbmy
            }
            else if (task->state == TaskState::DONE) {
                //sytuacja, w której z kolejki przyszedł węzeł, który został zmergowany przez brata
            }
            else {
//                int a = 0;
//#pragma omp critical(output)
//                cout<<"invalid task picked from queue"<<endl;
////                throw "invalid task picked from queue";
            }
        }

    }
    return finalResult;
}