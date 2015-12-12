#include "impl.h"
#include <math.h>

bool IntProblem::testDivide(Arg a, Arg b) {
    return false;
}

Arg IntProblem::merge(Arg a, Arg b) {
    return 0;
}

Arg IntProblem::compute(Arg a, Arg b) {
    return 0;
}

DividedData<Arg> IntProblem::divide(Arg a, Arg b) {
    return DividedData<Arg>();
}

Arg IntProblem::f(Arg x) {
    return sin(x + 2) * ((cos(3 * x - 2))) / 0.1 * x;
}


