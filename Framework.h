#ifndef DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H
#define DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H

template<typename DataType>
struct DividedData {
public:
    DataType a1;
    DataType b1;
    DataType a2;
    DataType b2;
};

template<typename DataType, typename ResultType>
class ProblemImpl {
public:
    virtual bool testDivide(DataType a, DataType b);

    virtual ResultType merge(ResultType a, ResultType b);

    virtual ResultType compute(DataType a, DataType b);

    virtual DividedData<DataType> divide(DataType a, DataType b);
};

#endif //DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H
