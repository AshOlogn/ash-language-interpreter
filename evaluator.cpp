#include <iostream>
#include <cstdint>
#include "token.h"
#include "typehandler.h"
#include "parsenode.h"

//recursively evaluate AST, start at root
ParseData evaluate(AbstractExpressionNode* node) {
  node->evaluate();
}


//evaluate expression appropriately depending on type of AbstractExpressionNode
ParseData evaluateUnaryExpression(AbstractExpressionNode* node) {
  ParseData d;
  return d;
}

ParseData evaluateArithmeticExpression(AbstractExpressionNode* node) {
  
  ParseData d;
  return d;
}

ParseData evaluateBitLogicalExpression(AbstractExpressionNode* node) {
  
  ParseData d;
  return d;
}

ParseData evaluateComparisonExpression(AbstractExpressionNode* node) {
  
  ParseData d;
  return d;
}

ParseData evaluateAssignmentExpression(AbstractExpressionNode* node) {
  
  ParseData d;
  return d;
}

ParseData evaluateLiteralExpression(AbstractExpressionNode* node) {
  
  ParseData d;
  return d;
}

ParseData evaluateGroupedExpression(AbstractExpressionNode* node) {
  ParseData d;
  return d;
  
}

