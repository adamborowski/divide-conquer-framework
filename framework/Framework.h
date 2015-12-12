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
    virtual bool testDivide(DataType a, DataType b) = 0;

    virtual ResultType merge(ResultType a, ResultType b) = 0;

    virtual ResultType compute(DataType a, DataType b) = 0;

    virtual DividedData<DataType> divide(DataType a, DataType b) = 0;
};

#endif //DIVIDE_CONQUER_FRAMEWORK_FRAMEWORK_H
