#include <iostream>
#include <execinfo.h>
#include "test/Problem.h"
#include "test/Timer.h"
#include <signal.h>
#include <stdlib.h>

using namespace std;

struct Args {
    bool debug;
    bool optimized;
    int numThreads;
    int threadsPerQueue;
    int parallelFactor;
    double startParam;
    double endParam;
    int chunkSize;
    int initialQueueSize;
    int queueChangeFactor;
};

void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}


Args parseArguments(int argc, const char *const *argv) {
    Args args;
    args.debug=false;
    args.optimized=true;
    args.numThreads=atoi(argv[1]);
    printf("using %d threads\n", args.numThreads);

    args.startParam=0;
    args.endParam=10000;
    args.threadsPerQueue=3;
    args.parallelFactor=100;
    args.chunkSize=100000;
    args.initialQueueSize=1000;
    args.queueChangeFactor=3;


    return args;
}

/**
 * usage
 * program [base_or_optimized] [numThreads] [threadsPerQueue] [parallelFactor]
 */
int main(int argc, char **argv) {
    srand (time(NULL));
    signal(SIGSEGV, handler);   // install our handler
    Args config = parseArguments(argc, argv);

    IntProblem p;
    AbstractProblemSolver<IntParam, IntResult> *solver;
    if (!config.optimized) {
        cout << "\nrunning base solver";
        solver = new BaseProblemSolver<IntParam, IntResult>(p, config.numThreads);
    }
    else {
        cout << "\nrunning optimized solver";
        solver = new OptimizedProblemSolver<IntParam, IntResult>(p, config.numThreads, config.threadsPerQueue, config.parallelFactor, config.chunkSize, config.initialQueueSize, config.queueChangeFactor);
    }

    // common config
    solver->debug = config.debug;


    // execution
    Timer timer;
    ThreadStats &threadStats = solver->getThreadStats();
    timer.start();
    IntResult result = solver->process({config.startParam, config.endParam});
    timer.stop();

    threadStats.calculate();
    cout << "\nresult: " << result
    << "\nduration: " << timer.getDurationString()
    << "\naverage thread load: " << threadStats.getAverage()
    << "\nstd deviation: " << threadStats.getStdDeviation()
    << "\n";

    // cleanup
    delete solver;
    return 0;
}