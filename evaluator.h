#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "token.h"
#include "parsenode.h"

//recursively evaluate AST, start at root
ParseData evaluate(AbstractExpressionNode* node);

//evaluate expression appropriately depending on type of AbstractExpressionNode
ParseData evaluateUnaryExpression(AbstractExpressionNode* node);

ParseData evaluateArithmeticExpression(ArithmeticOperatorNode* node);

ParseData evaluateBitLogicalExpression(AbstractExpressionNode* node);

ParseData evaluateComparisonExpression(AbstractExpressionNode* node);

ParseData evaluateAssignmentExpression(AbstractExpressionNode* node);

ParseData evaluateLiteralExpression(AbstractExpressionNode* node);

ParseData evaluateGroupedExpression(AbstractExpressionNode* node);

#endif
