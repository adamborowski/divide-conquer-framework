#include "Framework.h"
#include <omp.h>
#include <iostream>
#include "ThreadUnsafeLockFreeFactory.h"
//#include "ThreadUnsafeLockFreeFactory.cpp"
#include "ThreadUnsafeLockFreeQueue.h"
#include "ThreadUnsafeLockFreeQueue.cpp"
#include "SharedQueue.h"
#include "SharedQueue.cpp"
#include "RandomBucketAccessor.h"
#include "RandomBucketAccessor.cpp"

#include <unistd.h>

//#define PRINT_LOAD_REPORT

using namespace std;


template<class TParams, class TResult>
class ThreadSafeTaskFactory {
    vector<ThreadUnsafeLockFreeFactory<Task<TParams, TResult>>> factories;
public:
    ThreadSafeTaskFactory(int numThreads, int chunkSize)
            : factories(numThreads, chunkSize) {
    };

    Task<TParams, TResult> *create(const int threadId) {
        Task<TParams, TResult> *a;
        a = factories[threadId].create();
        return a;
    }

    int getNumCreatedTasks() {
        int sum = 0;
        for (int i = 0; i < factories.size(); i++) {
            sum += factories[i].getNumCreatedElements();
        }
        return sum;
    }
};


template<class TParams, class TResult>
TResult OptimizedProblemSolver<TParams, TResult>::process(TParams params) {
    ThreadSafeTaskFactory<TParams, TResult> taskFactory(this->numThreads, chunkSize);
    RandomBucketAccessor<SharedQueue<TaskPtr>> accessor(numThreads, threadsPerQueue, queueChangeFactor, initialQueueSize);

    omp_set_num_threads(this->numThreads);
    TResult finalResult;
    bool work = true;

    // create initial task
    TaskPtr rootTask = taskFactory.create(0);
    rootTask->parent = nullptr;
    rootTask->brother = nullptr;
    rootTask->isRootTask = true;
    rootTask->params = params;
    rootTask->state = TaskState::AWAITING;
    accessor.getInputBucket(0)->put(rootTask);
    ThreadStats &threadStats = this->threadStats;
    TaskPtr *inputTaskBatchesForAllThreads = new TaskPtr[parallelFactor * numThreads];
    TaskPtr *outputTaskBatchesForAllThreads = new TaskPtr[parallelFactor * numThreads * 2]; // for every input task we can produce maximum 2 sub-tasks

#pragma omp parallel
    {
        const int threadId = omp_get_thread_num();

        SharedQueue<TaskPtr> *inputQueue = accessor.getInputBucket(threadId);

        //every thread iterates over common queue and processes each task
        this->output("started working");
        TaskPtr *inputTaskBatch = &inputTaskBatchesForAllThreads[parallelFactor * threadId];
        TaskPtr *outputTaskBatch = &outputTaskBatchesForAllThreads[parallelFactor * threadId * 2];
        TaskPtr task;
        int numTaskPicked;
        int numTaskPut;
        while (work) {
            inputQueue->pickMany(inputTaskBatch, parallelFactor, numTaskPicked);
            numTaskPut = 0;

            if (numTaskPicked == 0) {
                this->output("skip, no tasks!");
                usleep(100000);
                continue;
            }
            else {
                threadStats.tickMany(threadId, numTaskPicked);
                this->output("picked " + to_string(numTaskPicked) + " tasks. Queue count is: " + to_string(inputQueue->getCountNotSynchronized()));
            }
            for (int i = 0; i < numTaskPicked; i++) {
                task = inputTaskBatch[i];
                if (task->state == TaskState::DONE || task->state == TaskState::DEAD) {
                    //we assume that is the second node from merge
                    task->state = TaskState::DEAD;
                    continue;
                }
                if (task->isRootTask) {
                    this->output("got root task!");
                }

                string common = "got task (" + to_string(task->params.a) + " " +
                                to_string(task->params.b) + ") / ";

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
                        Task<TParams, TResult> *task1 = taskFactory.create(threadId);
                        Task<TParams, TResult> *task2 = taskFactory.create(threadId);

                        task1->state = TaskState::AWAITING;
                        task2->state = TaskState::AWAITING;

                        task1->parent = task;
                        task2->parent = task;
                        task1->brother = task2;
                        task2->brother = task1;

                        const DividedParams<TParams> &dividedParams = this->problem.divide(task->params);
                        task1->params = dividedParams.param1;
                        task2->params = dividedParams.param2;
                        outputTaskBatch[numTaskPut++] = task1;
                        outputTaskBatch[numTaskPut++] = task2;
                    }
                    else {
                        task->result = this->problem.compute(task->params); // calculate directly the result
                        task->state = TaskState::COMPUTED; // change the state to "ready to merge"
                        this->output(common + "compute = ~" + to_string(task->result));
                        outputTaskBatch[numTaskPut++] = task; // put task to be merged later
                    }
                }
                else if (task->parent != nullptr and task->state == TaskState::COMPUTED and
                         task->brother->state == TaskState::COMPUTED) {

                    //we have two brothers ready to merge, put parent task to queue and mark it as CALCULATED
                    Task<TParams, TResult> *parent = task->parent;
                    parent->result = this->problem.merge(task->result, task->brother->result);
                    this->output(common + "merge brothers ~" + to_string(task->result) + " and ~" +
                                 to_string(task->brother->result) + " = " + to_string(parent->result));
                    parent->state = TaskState::COMPUTED;
                    task->state = TaskState::DEAD; // we know that task doesn't exist in queue so it will be always dead
                    task->brother->state = TaskState::DONE; // we don't know if brother is in queue
                    //put parent into queue again
                    outputTaskBatch[numTaskPut++] = parent;
                }
                else if (task->parent != nullptr and task->state == TaskState::COMPUTED and
                         task->brother->state == TaskState::AWAITING) {
                    this->output("sytuacja, w której nie mamy jeszcze drugiego brata gotowego, zatem nic nie róbmy");
                }
                else if (task->state == TaskState::DONE) {
                    this->output("sytuacja, w której z kolejki przyszedł węzeł, który został zmergowany przez brata");
                }
                else {
                    this->output("invalid task picked from queue");
                }
            }
            accessor.getOutputBucket(threadId)->putMany(outputTaskBatch, numTaskPut);
            this->output("put " + to_string(numTaskPut) + " tasks.");
        }
    }
#ifdef PRINT_LOAD_REPORT
    cout << "\nCreated " << taskFactory.getNumCreatedTasks() << " tasks";

    SharedQueue<TaskPtr> *queues = accessor.getBuckets();
    const int numQueues = accessor.getNumBuckets();
    cout << "\nQueue traffic:";
    for (int i = 0; i < numQueues; i++) {
        cout << "\nQueue " << i << ": " << queues[i].getPutCountNotSynchronized() << " picks";
    }
    cout << "\nThread traffic:";
    long sum = 0;
    for (int i = 0; i < numThreads; i++) {
        cout << "\nThread " << i << ": " << threadStats.getThreadTicks(i) << " picks";
    }
#endif

    return finalResult;
}