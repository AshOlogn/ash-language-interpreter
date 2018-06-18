#include <iostream>
#include <vector>
#include <unordered_map>
#include "parsetoken.h"
#include "parsenode.h"
#include "statementnode.h"
#include "typehandler.h"
#include "symboltable.h"
#include "executor.h"
#include "casteval.h"

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
	
	//leave the block scope
	node->symbolTable->leaveScope();
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


void executeNewAssignmentStatement(NewAssignmentStatementNode* node) {
  
  //get stuff out of the node first
  SymbolTable* symbolTable = node->symbolTable;
  char* variable = node->variable;
  ParseDataType type = node->type;
  
  //add variable to table
  if(node->value != NULL) {
    ParseData d = node->value->evaluate();
      
    //consider implicit casting
    symbolTable->declare(variable, castHelper(d, type));
  } else {
    
    ParseData d;
    d.type = node->value->evalType;
    symbolTable->declare(variable, castHelper(d, type));
  }
}

void executeAssignmentStatement(AssignmentStatementNode* node) {
	
  //get stuff out of the node first
  SymbolTable* symbolTable = node->symbolTable;
  char* variable = node->variable;
  AbstractExpressionNode* value = node->value;
  
  ParseDataType type = (symbolTable->get(variable)).type;
  ParseData d = value->evaluate();
  
  //update variable value (innermost scope)
  symbolTable->update(variable, castHelper(d, type));
}


