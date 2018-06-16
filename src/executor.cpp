#include <iostream>
#include <vector>
#include "parsetoken.h"
#include "parsenode.h"
#include "statementnode.h"
#include "typehandler.h"
#include "executor.h"

void executeExpressionStatement(ExpressionStatementNode* node) {
  node->expression->evaluate();
}

void executePrintStatement(PrintStatementNode* node) {
  std::cout << toStringParseData(node->expression->evaluate());
}

void executePrintLineStatement(PrintLineStatementNode* node) {
  std::cout << toStringParseData(node->expression->evaluate()) << std::endl;  
}

void executeGroupedStatement(GroupedStatementNode* node) {
  
  //symbol table enters new scope
  node->symbolTable->enterNewScope();
  
  //sequentially execute statements
  std::vector<AbstractStatementNode*>* statements = node->statements;
  std::vector<AbstractStatementNode*>::iterator it;
  
  for(it = statements->begin(); it != statements->end(); it++) {
    (*it)->execute();
  }
}

void executeConditionalStatement(ConditionalStatementNode* node) {
  
  std::vector<AbstractExpressionNode*>* conditions = node->conditions;
  std::vector<AbstractStatementNode*>* statements = node->statements;
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
