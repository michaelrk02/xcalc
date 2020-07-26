#pragma once

#include <stack>
#include <string>

#include "Expression.h"

enum calcstate_t {
    calcstate_EXPR,
    calcstate_BREAK,
    calcstate_INT,
    calcstate_REAL,
    calcstate_ERROR
};

enum opstate_t {
    opstate_ADD_SUB,
    opstate_MUL_DIV,
    opstate_POW,
    opstate_UNARY
};

class Calculator {
public:

    Calculator(const std::string &str);
    ~Calculator(void);

    double getResult(void);

private:

    int parentheses;
    calcstate_t state;
    std::stack<IExpression *> exprStack;
    double result;

    calcstate_t feed(char ch);

    void insertNum(char numch);
    void insertOp(char opch);

    opstate_t getOpState(void);

    void aggregate(void);
    void aggregatePow(void);
    void aggregateUnary(void);
    void aggregateAll(void);

};

