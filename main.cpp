#include <iostream>
#include "impl.h"

using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    IntProblem p;
    cout << p.f(3);
    ProblemSolver<Arg,Arg> solver(p,10);
    solver.start();

    return 0;
}
