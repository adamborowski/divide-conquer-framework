#include <iostream>
#include "Framework.h"
#include <math.h>

using namespace std;
typedef double Arg;

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}

class MaxProblem : ProblemImpl<Arg, Arg> {
public:
    Arg fun(Arg x);

    

    virtual bool testDivide(Arg a, Arg b) override;

    virtual Arg merge(Arg a, Arg b) override;

    virtual Arg compute(Arg a, Arg b) override;

    virtual DividedData divide(Arg a, Arg b) override;
};

bool MaxProblem::testDivide(Arg a, Arg b) {
    return ProblemImpl::testDivide(a, b);
}

Arg MaxProblem::merge(Arg a, Arg b) {
    return ProblemImpl::merge(a, b);
}

Arg MaxProblem::compute(Arg a, Arg b) {
    return ProblemImpl::compute(a, b);
}

DividedData MaxProblem::divide(Arg a, Arg b) {
    return ProblemImpl::divide(a, b);
}

Arg MaxProblem::fun(Arg x) {
    return sin(x + 2) * ((cos(3 * x - 2))) / 0.1 * x;
}
