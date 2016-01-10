#ifndef PROBLEM_H
#define PROBLEM_H

#include <math.h>
#include "../framework/Framework.h"
#include "../framework/Framework.cpp"
#include "../framework/AbstractProblemSolver.cpp"
#include "../framework/BaseProblemSolver.cpp"
#include "../framework/OptimizedProblemSolver.cpp"

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
        double middle = param.middle();
        IntResult big = compute(param);
        IntResult small = compute(param.a, middle) + compute(middle, param.b);
        double range = fabs(param.a - param.b);
        IntResult error = big - small;
        return
                range > .1
                or
                error > 0.003;
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
//        return 5 * x * x;
//        return fabs(x - 5);
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

#endif