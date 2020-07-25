#pragma once

#include "Expression.h"

#include <cstddef>

class UnaryExpression :
    public IExpression {
public:

    UnaryExpression(bool negation = false, IExpression *child = NULL);
    ~UnaryExpression(void);

    expr_t getType(void);
    double evaluate(void);
    void destroy(void);

    bool isNegation(void);
    void setNegation(bool negation);

    IExpression *getChild(void);
    void setChild(IExpression *child);

private:

    bool negation;
    IExpression *child;

};

