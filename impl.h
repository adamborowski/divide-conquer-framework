#include "framework/Framework.h"

typedef double Arg;

class IntProblem : public ProblemImpl<Arg, Arg> {

public:
    virtual bool testDivide(Arg a, Arg b);

    virtual Arg merge(Arg a, Arg b);

    virtual Arg compute(Arg a, Arg b);

    virtual DividedData<Arg> divide(Arg a, Arg b);

    Arg f(Arg x);
};