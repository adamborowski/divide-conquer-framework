#include <iostream>
#include "Tester.h"
#include "Timer.h"
#include "Problem.h"
#include "../framework/Framework.cpp"
#include "../framework/ProblemSolver.cpp"

//todo generalize tester (turn into template class) to pass any type of problemImpl
void Tester::run(const int numRepeats) {

    IntProblem baseProblem;
    ProblemSolver<IntParam, IntResult> baseSolver(baseProblem, false);

    Timer timer;
    AverageTimer averageTimer;


    for (int i = 0; i < numRepeats; i++) {

        timer.start();
        averageTimer.start();
        baseSolver.process({0, 1000}, 10);
//        cout << "\n THE FINAL RESULT: " << baseSolver.process({0, 100});
        timer.stop();
        averageTimer.stop();
        cout << "\nelapsed time: " << timer.getDurationString() << ".";
    }
    cout << "\naverage time: " << averageTimer.getAverageDurationString() << ".";
}
