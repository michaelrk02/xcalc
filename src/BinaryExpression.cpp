#include "BinaryExpression.h"

#include <cmath>
#include <iostream>

BinaryExpression::BinaryExpression(binop_t op, IExpression *leftExpr, IExpression *rightExpr) {
    this->setOp(op);
    this->setLeftExpr(leftExpr);
    this->setRightExpr(rightExpr);
}

BinaryExpression::~BinaryExpression(void) {
    if (this->leftExpr != NULL) {
        this->leftExpr->destroy();
    }
    if (this->rightExpr != NULL) {
        this->rightExpr->destroy();
    }
}

expr_t BinaryExpression::getType(void) {
    return expr_BINARY;
}

double BinaryExpression::evaluate(void) {
    if ((this->leftExpr == NULL) || (this->rightExpr == NULL)) {
        throw "evaluating NULL expression(s)";
    }

    switch (this->op) {
    case binop_ADD:
        return this->leftExpr->evaluate() + this->rightExpr->evaluate();
    case binop_SUB:
        return this->leftExpr->evaluate() - this->rightExpr->evaluate();
    case binop_MUL:
        return this->leftExpr->evaluate() * this->rightExpr->evaluate();
    case binop_DIV:
        return this->leftExpr->evaluate() / this->rightExpr->evaluate();
    case binop_POW:
        return std::pow(this->leftExpr->evaluate(), this->rightExpr->evaluate());
    }

    return 0.0;
}

void BinaryExpression::destroy(void) {
    delete this;
}

binop_t BinaryExpression::getOp(void) {
    return this->op;
}

void BinaryExpression::setOp(binop_t op) {
    this->op = op;
}

IExpression *BinaryExpression::getLeftExpr(void) {
    return this->leftExpr;
}

void BinaryExpression::setLeftExpr(IExpression *leftExpr) {
    this->leftExpr = leftExpr;
}

IExpression *BinaryExpression::getRightExpr(void) {
    return this->rightExpr;
}

void BinaryExpression::setRightExpr(IExpression *rightExpr) {
    this->rightExpr = rightExpr;
}

