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
  
}


BinaryOperatorNode::BinaryOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r) {
  operation = op;
  leftArg = l;
  rightArg = r;
}

ParseData BinaryOperatorNode::evaluate() {
  
}

///////////////////////////////////
///////      Literal        ///////
///////////////////////////////////

LiteralNode::LiteralNode(ParseData d) {
  data = d;
}

ParseData LiteralNode::evaluate() {
  
}


///////////////////////////////////
///////      Grouped        ///////
///////////////////////////////////

GroupedExpressionNode::GroupedExpressionNode(AbstractExpressionNode* exp) {
  closedExpression = exp;
}

ParseData GroupedExpressionNode::evaluate() {
  
}



