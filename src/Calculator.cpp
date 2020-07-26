#include "Calculator.h"

#include <cstddef>

#include "UnaryExpression.h"
#include "NumericExpression.h"
#include "BinaryExpression.h"

#define IS_NUM(ch) ((ch >= '0') && (ch <= '9'))
#define IS_ADD_SUB(ch) ((ch == '+') || (ch == '-'))
#define IS_MUL_DIV(ch) ((ch == '*') || (ch == '/'))
#define IS_POW(ch) (ch == '^')
#define IS_OP(ch) (IS_ADD_SUB(ch) || IS_MUL_DIV(ch) || IS_POW(ch))

#define IS_ADD_SUB_OP(op) ((op == binop_ADD) || (op == binop_SUB))
#define IS_MUL_DIV_OP(op) ((op == binop_MUL) || (op == binop_DIV))
#define IS_POW_OP(op) (op == binop_POW)

Calculator::Calculator(const std::string &str) {
    this->parentheses = 0;
    this->state = calcstate_EXPR;

    this->exprStack.push(NULL);
    for (int i = 0; i < str.size(); i++) {
        this->state = this->feed(str[i]);
    }
    this->state = this->feed(0x00);

    if (this->state == calcstate_ERROR) {
        throw "check your syntax and try again";
    }

    this->result = this->exprStack.top()->evaluate();
}

Calculator::~Calculator(void) {
    while (!this->exprStack.empty()) {
        IExpression *expr = this->exprStack.top();
        if (expr != NULL) {
            expr->destroy();
        }
        exprStack.pop();
    }
}

double Calculator::getResult(void) {
    return this->result;
}

calcstate_t Calculator::feed(char ch) {
    if (ch == '(') {
        this->parentheses++;
    } else if (ch == ')') {
        if (this->parentheses > 0) {
            this->parentheses--;
        } else {
            return calcstate_ERROR;
        }
    }

    if (this->state == calcstate_EXPR) {
        if (IS_NUM(ch)) {
            NumericExpression *expr = new NumericExpression();
            expr->append(ch);
            if (this->exprStack.top() == NULL) {
                this->exprStack.pop();
                this->exprStack.push(expr);
            } else {
                if (this->exprStack.top()->getType() == expr_UNARY) {
                    UnaryExpression *uexpr = reinterpret_cast<UnaryExpression *>(this->exprStack.top());
                    uexpr->setChild(expr);
                }
                if (this->exprStack.top()->getType() == expr_BINARY) {
                    BinaryExpression *bexpr = reinterpret_cast<BinaryExpression *>(this->exprStack.top());
                    bexpr->setRightExpr(expr);
                }
            }
            return calcstate_INT;
        }
        if (ch == '(') {
            if (this->exprStack.top() == NULL) {
                this->exprStack.pop();
                UnaryExpression *expr = new UnaryExpression();
                this->exprStack.push(expr);
            }
            this->exprStack.push(NULL);
            return calcstate_EXPR;
        }
        if (ch == '-') {
            if (this->exprStack.top() == NULL) {
                this->exprStack.pop();
            }
            UnaryExpression *expr = new UnaryExpression(true);
            this->exprStack.push(expr);
            return calcstate_EXPR;
        }
        if ((ch == 0x00) && (this->parentheses == 0)) {
            this->aggregateAll();
            return calcstate_EXPR;
        }
    }
    if (this->state == calcstate_BREAK) {
        if (IS_OP(ch)) {
            this->insertOp(ch);
            return calcstate_EXPR;
        }
        if (ch == ')') {
            this->aggregatePow();
            this->aggregateUnary();
            this->aggregate();
            return calcstate_BREAK;
        }
        if ((ch == 0x00) && (this->parentheses == 0)) {
            this->aggregateAll();
            return calcstate_EXPR;
        }
    }
    if (this->state == calcstate_INT) {
        if (IS_NUM(ch)) {
            this->insertNum(ch);
            return calcstate_INT;
        }
        if (ch == '.') {
            this->insertNum(ch);
            return calcstate_REAL;
        }
        if (IS_OP(ch)) {
            this->insertOp(ch);
            return calcstate_EXPR;
        }
        if (ch == ')') {
            this->aggregatePow();
            this->aggregateUnary();
            this->aggregate();
            return calcstate_BREAK;
        }
        if ((ch == 0x00) && (this->parentheses == 0)) {
            this->aggregateAll();
            return calcstate_EXPR;
        }
    }
    if (this->state == calcstate_REAL) {
        if (IS_NUM(ch)) {
            this->insertNum(ch);
            return calcstate_REAL;
        }
        if (IS_OP(ch)) {
            this->insertOp(ch);
            return calcstate_EXPR;
        }
        if (ch == ')') {
            this->aggregatePow();
            this->aggregateUnary();
            this->aggregate();
            return calcstate_BREAK;
        }
        if ((ch == 0x00) && (this->parentheses == 0)) {
            this->aggregateAll();
            return calcstate_EXPR;
        }
    }
    return calcstate_ERROR;
}

void Calculator::insertNum(char numch) {
    IExpression *top = this->exprStack.top();
    if (top->getType() == expr_NUMERIC) {
        NumericExpression *expr = reinterpret_cast<NumericExpression *>(top);
        expr->append(numch);
    } else if (top->getType() == expr_UNARY) {
        UnaryExpression *expr = reinterpret_cast<UnaryExpression *>(top);
        if (expr->getChild() != NULL) {
            if (expr->getChild()->getType() == expr_NUMERIC) {
                NumericExpression *nexpr = reinterpret_cast<NumericExpression *>(expr->getChild());
                nexpr->append(numch);
            } else {
                throw "inserting num on non-numeric child expression of an unary expression";
            }
        } else {
            throw "inserting num on NULL child expression of an unary expression";
        }
    } else if (top->getType() == expr_BINARY) {
        BinaryExpression *expr = reinterpret_cast<BinaryExpression *>(top);
        if (expr->getRightExpr() != NULL) {
            if (expr->getRightExpr()->getType() == expr_NUMERIC) {
                NumericExpression *nexpr = reinterpret_cast<NumericExpression *>(expr->getRightExpr());
                nexpr->append(numch);
            } else {
                throw "inserting num on non-numeric right expression of a binary expression";
            }
        } else {
            throw "inserting num on NULL right expression of a binary expression";
        }
    }
}

void Calculator::insertOp(char opch) {
    binop_t op;
    switch (opch) {
    case '+':
        op = binop_ADD;
        break;
    case '-':
        op = binop_SUB;
        break;
    case '*':
        op = binop_MUL;
        break;
    case '/':
        op = binop_DIV;
        break;
    case '^':
        op = binop_POW;
        break;
    }

    if (IS_ADD_SUB(opch)) {
        this->aggregatePow();
        this->aggregateUnary();

        IExpression *top = this->exprStack.top();
        BinaryExpression *expr;
        if ((top->getType() == expr_BINARY) && (this->getOpState() == opstate_MUL_DIV)) {
            this->aggregate();
            expr = new BinaryExpression(op, this->exprStack.top(), NULL);
        } else {
            expr = new BinaryExpression(op, top, NULL);
        }

        this->exprStack.pop();
        this->exprStack.push(expr);
    }
    if (IS_MUL_DIV(opch)) {
        this->aggregatePow();
        this->aggregateUnary();

        IExpression *top = this->exprStack.top();
        if (this->getOpState() == opstate_ADD_SUB) {
            if (top->getType() == expr_BINARY) {
                BinaryExpression *leftExpr = reinterpret_cast<BinaryExpression *>(top);
                BinaryExpression *expr = new BinaryExpression(op, leftExpr->getRightExpr(), NULL);
                this->exprStack.push(expr);
            }
        } else if (this->getOpState() == opstate_MUL_DIV) {
            this->exprStack.pop();
            BinaryExpression *expr = new BinaryExpression(op, top, NULL);
            this->exprStack.push(expr);
        }
    }
    if (IS_POW(opch)) {
        IExpression *top = this->exprStack.top();
        if (top->getType() == expr_BINARY) {
            BinaryExpression *leftExpr = reinterpret_cast<BinaryExpression *>(top);
            BinaryExpression *expr = new BinaryExpression(op, leftExpr->getRightExpr(), NULL);
            this->exprStack.push(expr);
        } else {
            this->exprStack.pop();
            BinaryExpression *parent = new BinaryExpression(op, top, NULL);
            this->exprStack.push(parent);
            this->exprStack.push(NULL);
        }
    }
}

opstate_t Calculator::getOpState(void) {
    IExpression *top = this->exprStack.top();
    if (top->getType() == expr_BINARY) {
        BinaryExpression *expr = reinterpret_cast<BinaryExpression *>(top);
        if (IS_ADD_SUB_OP(expr->getOp())) {
            return opstate_ADD_SUB;
        }
        if (IS_MUL_DIV_OP(expr->getOp())) {
            return opstate_MUL_DIV;
        }
    } else {
        if (this->exprStack.size() > 1) {
            this->exprStack.pop();
            IExpression *self = this->exprStack.top();
            this->exprStack.push(top);
            if (self->getType() == expr_BINARY) {
                BinaryExpression *expr = reinterpret_cast<BinaryExpression *>(self);
                if (IS_POW_OP(expr->getOp())) {
                    return opstate_POW;
                }
            } else if (self->getType() == expr_UNARY) {
                return opstate_UNARY;
            }
        }
    }
    return opstate_MUL_DIV;
}

void Calculator::aggregate(void) {
    if (this->exprStack.size() > 1) {
        IExpression *top = this->exprStack.top();
        this->exprStack.pop();
        IExpression *self = this->exprStack.top();
        this->exprStack.pop();
        if (self != NULL) {
            if (self->getType() == expr_BINARY) {
                BinaryExpression *expr = reinterpret_cast<BinaryExpression *>(self);
                expr->setRightExpr(top);
            } else if (self->getType() == expr_UNARY) {
                UnaryExpression *expr = reinterpret_cast<UnaryExpression *>(self);
                expr->setChild(top);
            } else {
                throw "aggregating on numerical expression";
            }
            this->exprStack.push(self);
        } else {
            throw "aggregating on a NULL expression";
        }
    } else {
        throw "trying to aggregate <2 sized expression stack";
    }
}

void Calculator::aggregateUnary(void) {
    while ((this->exprStack.size() > 1) && (this->getOpState() == opstate_UNARY)) {
        this->aggregate();
    }
    if (this->exprStack.size() > 1) {
        this->aggregate();
    }
}

void Calculator::aggregatePow(void) {
    while ((this->exprStack.size() > 1) && (this->getOpState() == opstate_POW)) {
        this->aggregate();
    }
    if (this->exprStack.size() > 1) {
        this->aggregate();
    }
}

void Calculator::aggregateAll(void) {
    this->aggregatePow();
    this->aggregateUnary();
    while (this->exprStack.size() > 1) {
        this->aggregate();
    }
}

