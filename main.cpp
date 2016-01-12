#include <boost/program_options.hpp>
#include <iostream>
#include <execinfo.h>
#include "test/Problem.h"
#include "test/Timer.h"
#include <signal.h>

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
    namespace po = boost::program_options;
    po::options_description desc("Divide & Conquer Framework demo");
    desc.add_options()
            ("debug,v", po::bool_switch(&args.debug)->default_value(false), "Debug mode")
            ("optimized,o", po::bool_switch(&args.optimized)->default_value(false), "Run optimized version")
            ("numThreads,n", po::value<int>(&args.numThreads)->default_value(1), "Number of threads")
            ("startParam,s", po::value<double>(&args.startParam)->default_value(0), "start param")
            ("endParam,e", po::value<double>(&args.endParam)->default_value(100), "endParam")
            ("threadsPerQueue,q", po::value<int>(&args.threadsPerQueue)->default_value(1),
             "Number of threads per queue for optimized version")
            ("chunkSize,c", po::value<int>(&args.chunkSize)->default_value(1000),
             "Lock free factory chunk size")
            ("help,h", "produce help message");
    ("parallelFactor,p", po::value<int>(&args.parallelFactor)->default_value(1), "Num task gained at once per thread.");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        cout << desc << "\n";
        exit (0);
    }
    return args;
}

/**
 * usage
 * program [base_or_optimized] [numThreads] [threadsPerQueue] [parallelFactor]
 */
int main(int argc, char **argv) {
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
        solver = new OptimizedProblemSolver<IntParam, IntResult>(p, config.numThreads, config.threadsPerQueue,
                                                                 config.parallelFactor, config.chunkSize);
    }

    // common config
    solver->debug = config.debug;


    // execution
    Timer timer;
    ThreadStats &threadStats = solver->getThreadStats();
    timer.start();
    IntResult d = solver->process({config.startParam, config.endParam});
    timer.stop();

    threadStats.calculate();
    cout << "\nresult: " << d
    << "\nduration: " << timer.getDurationString()
    << "\naverage load: " << threadStats.getAverage()
    << "\nstd deviation: " << threadStats.getStdDeviation()
    << "\n";

    // cleanup
    delete solver;
    return 0;
}