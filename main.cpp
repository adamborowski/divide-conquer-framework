#include <iostream>
#include <math.h>
#include "framework/Framework.h"

using namespace std;
struct Param{
    double a;
    double b;
};
typedef double Result;


class IntProblem : public ProblemImpl<Param, Result> {

public:
    virtual bool testDivide(Result a, Result b) {
        return false;
    }

    virtual Result merge(Result a, Result b) {
        return 0;
    }

    virtual Result compute(Result a, Result b) {
        return 0;
    }

    virtual DividedData<Result> divide(Result a, Result b) {
        DividedData<Result> d;
        return d;
    }

    Result f(Result x) {
        return sin(x + 2) * ((cos(3 * x - 2))) / 0.1 * x;
    }
};


int main() {
    cout << "Hello, World!" << endl;
    IntProblem p;
    cout << p.f(3);
    ProblemSolver<Result, Result> solver(p, 10);
    solver.process();

    return 0;
}