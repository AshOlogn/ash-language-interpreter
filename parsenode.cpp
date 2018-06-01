#include "parsetoken.h"
#include "parsenode.h"
#include "evaluator.h"

///////////////////////////////////
///////     Operations      ///////
///////////////////////////////////

UnaryOperatorNode::UnaryOperatorNode(ParseOperatorType op, AbstractExpressionNode* l) {
  operation = op;
  leftArg = l;  
}

ParseData UnaryOperatorNode::evaluate() {
  return evaluateUnaryExpression(this);
}

//class that represents arithmetic
ArithmeticOperatorNode::ArithmeticOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r) {
  operation = op;
  leftArg = l;
  rightArg = r;
}

ParseData ArithmeticOperatorNode::evaluate() { 
  return evaluateArithmeticExpression(this);
}

//class that represents bitwise and logical operations
BitLogicalOperatorNode::BitLogicalOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r) {
  operation = op;
  leftArg = l;
  rightArg = r;
}

ParseData BitLogicalOperatorNode::evaluate() { 
  return evaluateBitLogicalExpression(this);
}

//class that represents assignment
AssignmentOperatorNode::AssignmentOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r) {
  operation = op;
  leftArg = l;
  rightArg = r; 
}

ParseData AssignmentOperatorNode::evaluate() { 
  return evaluateAssignmentExpression(this);
}

//class that represents comparison
ComparisonOperatorNode::ComparisonOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r) { 
  operation = op;
  leftArg = l;
  rightArg = r;
}

ParseData ComparisonOperatorNode::evaluate() { 
  return evaluateComparisonExpression(this);
}


///////////////////////////////////
///////      Literal        ///////
///////////////////////////////////

LiteralNode::LiteralNode(ParseData d) {
  data = d;
}

ParseData LiteralNode::evaluate() { 
  return evaluateLiteralExpression(this);
}


///////////////////////////////////
///////      Grouped        ///////
///////////////////////////////////

GroupedExpressionNode::GroupedExpressionNode(AbstractExpressionNode* exp) {
  closedExpression = exp;
}

ParseData GroupedExpressionNode::evaluate() { 
  return evaluateGroupedExpression(this);
}



