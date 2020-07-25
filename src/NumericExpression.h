#pragma once

#include "Expression.h"

#include <sstream>

class NumericExpression :
    public IExpression {
public:

    NumericExpression(void);
    ~NumericExpression(void);

    expr_t getType(void);
    double evaluate(void);
    void destroy(void);

    void append(char ch);

private:

    std::ostringstream stream;

};

