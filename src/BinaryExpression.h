#pragma once

#include "Expression.h"

#include <cstddef>

enum binop_t {
    binop_ADD,
    binop_SUB,
    binop_MUL,
    binop_DIV,
    binop_POW
};

class BinaryExpression :
    public IExpression {
public:

    BinaryExpression(binop_t op, IExpression *leftExpr = NULL, IExpression *rightExpr = NULL);
    ~BinaryExpression(void);

    expr_t getType(void);
    double evaluate(void);
    void destroy(void);

    binop_t getOp(void);
    void setOp(binop_t op);

    IExpression *getLeftExpr(void);
    void setLeftExpr(IExpression *leftExpr);

    IExpression *getRightExpr(void);
    void setRightExpr(IExpression *rightExpr);

private:

    binop_t op;
    IExpression *leftExpr;
    IExpression *rightExpr;

};

