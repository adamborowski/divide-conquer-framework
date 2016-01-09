#include "test/Problem.h"
#include "test/Timer.h"

using namespace std;

struct Args {
    bool debug = false;
    bool baseOrOptimized = true;
    int numThreads = 1;
    int threadsPerQueue = 1;
    int parallelFactor = 1;
};

bool toBool(char *val) {
    return *val == '1';
}

int toInt(char *val) {
    return atoi(val);
}

Args parseArguments(int argc, char **argv) {
    Args args;
    if (argc > 1)
        args.debug = toBool(argv[1]);
    if (argc > 2)
        args.baseOrOptimized = toBool(argv[2]);
    if (argc > 3)
        args.numThreads = toInt(argv[3]);
    if (argc > 4)
        args.threadsPerQueue = toInt(argv[4]);
    if (argc > 5)
        args.parallelFactor = toInt(argv[5]);
    return args;
}

/**
 * usage
 * program [base_or_optimized] [numThreads] [threadsPerQueue] [parallelFactor]
 */
int main(int argc, char **argv) {
    Args config = parseArguments(argc, argv);

    IntProblem p;
    AbstractProblemSolver<IntParam, IntResult> *solver;
    if (config.baseOrOptimized) {
        solver = new BaseProblemSolver<IntParam, IntResult>(p, config.numThreads);
    }
    else {
//        solver = new OptimizedProblemSolver<IntParam, IntResult> (p, config.numThreads);
    }

    // common config
    solver->debug = config.debug;


    // execution
    Timer timer;
    timer.start();
    IntResult d = solver->process({0, 100});
    timer.stop();
    cout << timer.getDurationString() << endl; // us

    // cleanup
    delete solver;
    return 0;
}