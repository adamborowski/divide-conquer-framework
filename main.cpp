#include <boost/program_options.hpp>
#include <iostream>
#include "test/Problem.h"
#include "test/Timer.h"

using namespace std;

struct Args {
    bool debug;
    bool optimized;
    int numThreads;
    int threadsPerQueue;
    int parallelFactor;
};


Args parseArguments(int argc, const char *const *argv) {
    Args args;
    namespace po = boost::program_options;
    po::options_description desc("Divide & Conquer Framework demo");
    desc.add_options()
            ("debug,v", po::bool_switch(&args.debug)->default_value(false), "Debug mode")
            ("optimized,o", po::bool_switch(&args.optimized)->default_value(false), "Run optimized version")
            ("numThreads,n", po::value<int>(&args.numThreads)->default_value(1), "Number of threads");
            ("threadsPerQueue,q", po::value<int>(&args.threadsPerQueue)->default_value(1), "Number of threads per queue for optimized version");
            ("parallelFactor,p", po::value<int>(&args.parallelFactor)->default_value(1), "Num task gained at once per thread.");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
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
    if (!config.optimized) {
        solver = new BaseProblemSolver<IntParam, IntResult>(p, config.numThreads);
    }
    else {
//        solver = new OptimizedProblemSolver<IntParam, IntResult> (p, config.numThreads);
    }

    // common config
    solver->debug = config.debug;


    // execution
    Timer timer;
    ThreadStats &threadStats = solver->getThreadStats();
    timer.start();
    IntResult d = solver->process({0, 100});
    timer.stop();

    threadStats.calculate();
    cout << endl
    << timer.getDurationString() << endl
    << threadStats.getAverage() << endl
    << threadStats.getStdDeviation() << endl;

    // cleanup
    delete solver;
    return 0;
}