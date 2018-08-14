#ifndef ARITHMETICEVAL_H
#define ARITHMETICEVAL_H

#include "parsetoken.h"
#include "parsenode.h"

//evaluates ** * / % + -
ParseData evaluateArithmeticExpression(ArithmeticOperatorNode* node);

#endif