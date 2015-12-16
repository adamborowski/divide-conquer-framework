#include <iostream>
#include <math.h>

#include "framework/Framework.cpp"


using namespace std;

struct IntParam {
    double a;
    double b;

    double middle() {
        return (a + b) / 2;
    }
};

typedef double IntResult;


class IntProblem : public ProblemImpl<IntParam, IntResult> {

public:
    virtual bool testDivide(IntParam param) {


        return fabs(param.a - param.b) > 0.1 //range is to large
               or ( // division error is too large
                       compute(param)
                       - compute(param.a, (param.a + param.middle()))
                       - compute(param.middle(), param.b)
                       > 0.2
               );
    }

    virtual IntResult merge(IntResult a, IntResult b) {
        return a + b;
    }

    virtual IntResult compute(IntParam param) {
        return compute(param.a, param.b);
    }


    virtual DividedParams<IntParam> divide(IntParam param) {
        DividedParams<IntParam> d;
        d.param1.a = param.a;
        d.param1.b = (param.a + param.b) / 2;
        d.param2.a = d.param1.b;
        d.param2.b = param.b;
        return d;
    }

    IntResult f(IntResult x) {
        return sin(x + 2) * ((cos(3 * x - 2))) / 0.1 * x;
    }

    IntResult compute(IntResult a, IntResult b) {
        return (f(a) + f(b) * (b - a)) / 2;
    }
};


int main() {
    cout << "Hello, World!" << endl;
    IntProblem p;
    cout << p.f(3);
    ProblemSolver<IntParam, IntResult> solver(p, 10);
    solver.process();

    return 0;
}