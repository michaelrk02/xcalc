#include "UnaryExpression.h"

UnaryExpression::UnaryExpression(bool negation, IExpression *child) {
    this->setNegation(negation);
    this->setChild(child);
}

UnaryExpression::~UnaryExpression(void) {
    if (this->child != NULL) {
        this->child->destroy();
    }
}

expr_t UnaryExpression::getType(void) {
    return expr_UNARY;
}

double UnaryExpression::evaluate(void) {
    if (this->negation) {
        return -this->child->evaluate();
    }
    return this->child->evaluate();
}

void UnaryExpression::destroy(void) {
    delete this;
}

bool UnaryExpression::isNegation(void) {
    return this->negation;
}

void UnaryExpression::setNegation(bool negation) {
    this->negation = negation;
}

IExpression *UnaryExpression::getChild(void) {
    return this->child;
}

void UnaryExpression::setChild(IExpression *child) {
    this->child = child;
}

