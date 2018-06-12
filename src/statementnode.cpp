#include <iostream>
#include <vector>
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

//represents if-elif-else structure
ConditionalStatementNode::ConditionalStatementNode(std::vector<AbstractExpressionNode*>* cond, std::vector<AbstractStatementNode*>* stat) {
  conditions = cond;
  statements = stat;
}

void ConditionalStatementNode::execute() {
  
  std::cout << "called" << std::endl;

  std::vector<AbstractExpressionNode*>::iterator it1;
  std::vector<AbstractStatementNode*>::iterator it2;
  
  it1 = conditions->begin();
  it2 = statements->begin();
  
  for(; it1 != conditions->end(); it1++, it2++) {
    
    ParseData d = (*it1)->evaluate();
    if(d.type == BOOL_T && d.value.integer != 0) {
      
      (*it2)->execute();
      return;
    }
   
  }
  
  
}







