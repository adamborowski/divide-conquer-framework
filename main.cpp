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


        return fabs(param.a - param.b) > .2 //range is to large
//               or ( // division error is too large
//                       compute(param)
//                       - compute(param.a, (param.a + param.middle()))
//                       - compute(param.middle(), param.b)
//                       > 0.02
//               );
                ;
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
//        return sin(x + 2) * ((cos(3 * x - 2))) / 0.1 * x;
//        return 5 * x * x;
        return fabs(x - 5);
    }

    IntResult compute(IntResult a, IntResult b) {
        IntResult delta = b - a;

        IntResult fa = f(a);
        IntResult fb = f(b);
        return (fa + fb) / 2 * delta;
    }

    virtual std::string toString(IntParam params) {
        return "(" + to_string(params.a) + ", " + to_string(params.b) + ")";
    }

    virtual std::string toString(IntResult result) {
        return to_string(result);
    }
};


int main() {
    IntProblem p;
    ProblemSolver<IntParam, IntResult> solver(p, 10, true);
    cout << "\n THE FINAL RESULT: " << solver.process({0, 10});


    return 0;
}