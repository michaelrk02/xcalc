#pragma once

enum expr_t {
    expr_NUMERIC,
    expr_UNARY,
    expr_BINARY
};

class IExpression {
public:

    virtual expr_t getType(void) = 0;
    virtual double evaluate(void) = 0;
    virtual void destroy(void) = 0;

};

