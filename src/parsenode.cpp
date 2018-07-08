#include <string>
#include <algorithm>
#include <iostream>
#include "token.h"
#include "parsetoken.h"
#include "typehandler.h"
#include "evaluator.h"
#include "symboltable.h"
#include "function.h"
#include "array.h"
#include "parsenode.h"

///////////////////////////////////
///////     Assignment      ///////
///////////////////////////////////

AssignmentExpressionNode::AssignmentExpressionNode(std::string var, ParseDataType varType, AbstractExpressionNode* val, SymbolTable* table, uint32_t varLine) {

  variable = var;
  variableType = varType;
  value = val;
  symbolTable = table;
  
  evalType = varType;
  startLine = varLine;
  endLine = val->endLine;
}

ParseData AssignmentExpressionNode::evaluate() {
  return evaluateAssignmentExpression(this);
}

std::string AssignmentExpressionNode::toString() {
  
}


///////////////////////////////////
///////     Operations      ///////
///////////////////////////////////

//unary operations
UnaryOperatorNode::UnaryOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, uint32_t operatorLine) {
  operation = op;
  leftArg = l;
  evalType = getTypeUnaryExpression(op, l->evalType); 
  startLine = std::min(operatorLine, l->startLine);
  endLine = std::max(operatorLine, l->endLine);
}

UnaryOperatorNode::UnaryOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, SymbolTable* table, uint32_t operatorLine) {
  operation = op;
  leftArg = l;
  symbolTable = table;
  evalType = getTypeUnaryExpression(op, l->evalType); 
  startLine = std::min(operatorLine, l->startLine);
  endLine = std::max(operatorLine, l->endLine);
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
  startLine = l->startLine;
  endLine = r->endLine;
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
  startLine = l->startLine;
  endLine = r->endLine;
  evalType = getTypeBitLogicalExpression(op, l->evalType, r->evalType);
}

ParseData BitLogicalOperatorNode::evaluate() { 
  return evaluateBitLogicalExpression(this);
}


//class that represents comparison
ComparisonOperatorNode::ComparisonOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r) { 
  operation = op;
  leftArg = l;
  rightArg = r;
  startLine = l->startLine;
  endLine = r->endLine;
  evalType = getTypeComparisonExpression(op, l->evalType, r->evalType);
}

ParseData ComparisonOperatorNode::evaluate() { 
  return evaluateComparisonExpression(this);
}


///////////////////////////////////
///////        Cast         ///////
///////////////////////////////////

CastNode::CastNode(AbstractExpressionNode* e, ParseDataType type, uint32_t startLin) {
  expression = e;
  evalType = finalType = type;

  startLine = startLin;
  endLine = e->endLine;
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

ArrayAccessNode::ArrayAccessNode(AbstractExpressionNode* arr, AbstractExpressionNode* s, uint32_t endLin) {
  array = arr; start = s; endLine = endLin; isSlice = false;
  startLine = arr->startLine;
  endLine = endLin;
	evalType = arr->subType;
}

ArrayAccessNode::ArrayAccessNode(AbstractExpressionNode* arr, AbstractExpressionNode* s, AbstractExpressionNode* e, uint32_t endLin) {
  array = arr; start = s; end = e; endLine = endLin; isSlice = true;
  startLine = arr->startLine;
  endLine = endLin;
	evalType = arr->evalType;
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
///////        Arrays       ///////
///////////////////////////////////

//represents an array of primitive values
ArrayNode::ArrayNode(ParseDataType sType, bool isInit, AbstractExpressionNode* len, AbstractExpressionNode** vals) {
	evalType = ARRAY_T;
	subType = sType;
	isInitialized = isInit;
	length = len;
	values = vals;
}

ParseData ArrayNode::evaluate() {
	return evaluateArrayExpression(this);
}

std::string ArrayNode::toString() {
	std::string str = "";
	str.append("[length = ");
	str.append(length->toString());
	str.append(", type = ");
	str.append(toStringParseDataType(subType));
	str.append("]");
	return str;
}

///////////////////////////////////
///////      Variable       ///////
///////////////////////////////////

VariableNode::VariableNode(std::string var, SymbolTable* table, uint32_t line) {
  symbolTable = table;
  variable = var;
  startLine = endLine = line;
  evalType = (symbolTable->get(var)).type;
}

ParseData VariableNode::evaluate() {
  return symbolTable->get(variable);
}

std::string VariableNode::toString() {
  return std::string(variable);
}

///////////////////////////////////
///////    Function Call    ///////
///////////////////////////////////

FunctionExpressionNode::FunctionExpressionNode(uint32_t nArgs, AbstractExpressionNode** args, Function* f, SymbolTable* table) {
	numArgs = nArgs;
	arguments = args;
	function = f;
	symbolTable = table;
	evalType = f->returnType;

	isReturned = f->returnFlag;
	returnValue = f->returnValue;
}

ParseData FunctionExpressionNode::evaluate() {
	return evaluateFunctionExpression(this);
}

std::string FunctionExpressionNode::toString() {
	std::string str;
	return str;
}


///////////////////////////////////
///////      Literal        ///////
///////////////////////////////////

LiteralNode::LiteralNode(ParseData d, uint32_t line) {
  data = d;
  startLine = endLine = line;
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

GroupedExpressionNode::GroupedExpressionNode(AbstractExpressionNode* exp, uint32_t startLin, uint32_t endLin) {
  closedExpression = exp;
  startLine = startLin;
  endLine = endLin;
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

