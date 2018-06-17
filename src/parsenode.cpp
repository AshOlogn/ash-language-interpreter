#include <string>
#include <iostream>
#include "token.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "typehandler.h"
#include "evaluator.h"

///////////////////////////////////
///////     Operations      ///////
///////////////////////////////////

//unary operations
UnaryOperatorNode::UnaryOperatorNode(ParseOperatorType op, AbstractExpressionNode* l) {
  operation = op;
  leftArg = l; 
  evalType = getTypeUnaryExpression(op, l->evalType); 
}

ParseData UnaryOperatorNode::evaluate() {
  return evaluateUnaryExpression(this);
}

std::string UnaryOperatorNode::toString() {
  std::string str = "(";
  str.append(toStringParseOperatorType(operation));
  str.append(" ");
  str.append(leftArg->toString());
  str.append(")");
  return str;
}

//binary operation toString
std::string AbstractBinaryOperatorNode::toString() {
  std::string str = "( ";
  str.append(toStringParseOperatorType(operation));
  str.append(" ");
  str.append(leftArg->toString());
  str.append(" "); 
  str.append(rightArg->toString());
  str.append(")");
  return str;
}

//class that represents arithmetic
ArithmeticOperatorNode::ArithmeticOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r) {
  operation = op;
  leftArg = l;
  rightArg = r;
  evalType = getTypeArithmeticExpression(op, l->evalType, r->evalType);
}

ParseData ArithmeticOperatorNode::evaluate() { 
  return evaluateArithmeticExpression(this);
}

//class that represents bitwise and logical operations
BitLogicalOperatorNode::BitLogicalOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r) {
  operation = op;
  leftArg = l;
  rightArg = r;
  evalType = getTypeBitLogicalExpression(op, l->evalType, r->evalType);
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
  evalType = getTypeComparisonExpression(op, l->evalType, r->evalType);
}

ParseData ComparisonOperatorNode::evaluate() { 
  return evaluateComparisonExpression(this);
}


///////////////////////////////////
///////        Cast         ///////
///////////////////////////////////

CastNode::CastNode(AbstractExpressionNode* e, ParseDataType type) {
  expression = e;
  evalType = finalType = type;
}

std::string CastNode::toString() {
  std::string str = "(";
  str.append(toStringParseDataType(finalType));
  str.append(" ");
  str.append(expression->toString());
  str.append(")");
  return str;
}

ParseData CastNode::evaluate() {
  return evaluateCastExpression(this);
}


///////////////////////////////////
///////    Member Access    ///////
///////////////////////////////////

ArrayAccessNode::ArrayAccessNode(AbstractExpressionNode* arr, AbstractExpressionNode* s) {
  array = arr; start = s; isSlice = false;
}

ArrayAccessNode::ArrayAccessNode(AbstractExpressionNode* arr, AbstractExpressionNode* s, AbstractExpressionNode* e) {
  array = arr; start = s; end = e; isSlice = true;
}

ParseData ArrayAccessNode::evaluate() {
  return evaluateArrayAccess(this);
}

std::string ArrayAccessNode::toString() {
  
  std::string str = "([";
  if(isSlice) {
    
    str.append(start->toString());
    str.append(":");
    str.append(end->toString());
    str.append("] ");
    
  } else {
    str.append("]");
  }
 
  str.append(array->toString());
  str.append(")");
 
  return str;
}

///////////////////////////////////
///////      Variable       ///////
///////////////////////////////////

class VariableNode : public AbstractExpressionNode {
  public:
    SymbolTable* symbolTable;
    char* variable;
    VariableNode(char* var, SymbolTable* table);
};

VariableNode::VariableNode(char* var, SymbolTable* table) {
  symbolTable = table;
  variable = var;
}

ParseData VariableNode::evaluate() {
  return symbolTable->get(variable);
}

std::string VariableNode::toString() {
  return std::string(variable);
}

///////////////////////////////////
///////      Literal        ///////
///////////////////////////////////

LiteralNode::LiteralNode(ParseData d) {
  data = d;
  evalType = d.type;
}

ParseData LiteralNode::evaluate() { 
  return evaluateLiteralExpression(this);
}

std::string LiteralNode::toString() {
  return std::string(toStringParseData(data));
}

///////////////////////////////////
///////      Grouped        ///////
///////////////////////////////////

GroupedExpressionNode::GroupedExpressionNode(AbstractExpressionNode* exp) {
  closedExpression = exp;
  evalType = exp->evalType;
}

ParseData GroupedExpressionNode::evaluate() { 
  return evaluateGroupedExpression(this);
}

std::string GroupedExpressionNode::toString() {
  std::string str = "(";
  str.append(closedExpression->toString());
  str.append(")");
  return str;
}

