#include <iostream>
#include "parsetoken.h"
#include "parsenode.h"
#include "statementnode.h"

//represents a single-expression statement
ExpressionStatementNode::ExpressionStatementNode(AbstractExpressionNode* exp) {
  expression = exp;
}

void ExpressionStatementNode::execute() {
  expression->evaluate();
}

//represents print statement (no new line character)
PrintStatementNode::PrintStatementNode(AbstractExpressionNode* exp) {
  expression = exp;
}

void PrintStatementNode::execute() {
  std::cout << toStringParseData(expression->evaluate());
}

//represents print statement (yes new line character)
PrintLineStatementNode::PrintLineStatementNode(AbstractExpressionNode* exp) {
  expression = exp;
}

void PrintLineStatementNode::execute() {
  std::cout << toStringParseData(expression->evaluate()) << std::endl;
}