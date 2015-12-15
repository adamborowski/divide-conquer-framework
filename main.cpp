#include <iostream>
#include <math.h>
#include "framework/Framework.h"

using namespace std;
typedef double Arg;


class IntProblem : public ProblemImpl<Arg, Arg> {

public:
    virtual bool testDivide(Arg a, Arg b) {
        return false;
    }

    virtual Arg merge(Arg a, Arg b) {
        return 0;
    }

    virtual Arg compute(Arg a, Arg b) {
        return 0;
    }

    virtual DividedData<Arg> divide(Arg a, Arg b) {
        DividedData<Arg> d;
        return d;
    }

    Arg f(Arg x) {
        return sin(x + 2) * ((cos(3 * x - 2))) / 0.1 * x;
    }
};


int main() {
    cout << "Hello, World!" << endl;
    IntProblem p;
    cout << p.f(3);
    ProblemSolver<Arg, Arg> solver(p, 10);
    solver.start();

    return 0;
}