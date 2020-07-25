#include "NumericExpression.h"

#include <iostream>

NumericExpression::NumericExpression(void) {
}

NumericExpression::~NumericExpression(void) {
}

expr_t NumericExpression::getType(void) {
    return expr_NUMERIC;
}

double NumericExpression::evaluate(void) {
    double num;
    std::istringstream input(this->stream.str());
    input >> num;
    return num;
}

void NumericExpression::append(char ch) {
    this->stream << ch;
}

void NumericExpression::destroy(void) {
    delete this;
}

