#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>
#include "parsetoken.h"
#include "parsenode.h"
#include "statementnode.h"
#include "typehandler.h"
#include "symboltable.h"
#include "executor.h"
#include "casteval.h"
#include "array.h"

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
  std::string variable = node->variable;
  ParseDataType type = node->type;
  
  //add variable to table
  if(node->value != NULL) {

    ParseData d = node->value->evaluate();
		Array* array = (Array*) d.value.allocated;

    //consider implicit casting
    symbolTable->declare(variable, castHelper(d, type));
  } else {
    
    ParseData d;
    d.type = symbolTable->get(variable).type;
    symbolTable->declare(variable, castHelper(d, type));
  }
}

void executeAssignmentStatement(AssignmentStatementNode* node) {
	
  //get stuff out of the node first
  SymbolTable* symbolTable = node->symbolTable;
  std::string variable = node->variable;
  AbstractExpressionNode* value = node->value;
  
  ParseDataType type = (symbolTable->get(variable)).type;
  ParseData d = value->evaluate();
  
  //update variable value (innermost scope)
  symbolTable->update(variable, castHelper(d, type));
}

void executeArrayAssignmentStatement(ArrayAssignmentStatementNode* node) {

	//get stuff out of the node first
	SymbolTable* symbolTable = node->symbolTable;
	std::string variable = node->variable;
	int32_t index = (int32_t) node->index->evaluate().value.integer;
	ParseData value = node->value->evaluate();

	//get array from the symbol table
	Array* array = (Array*) (symbolTable->get(variable)).value.allocated;

	//do the assignment
	array->values[index] = value;
}

void executeFunctionStatement(FunctionStatementNode* node) {

	SymbolTable* symbolTable = node->symbolTable;
	std::string functionName = node->functionName;
	Function* function = node->function;

	ParseData d;
	d.type = FUN_T;
	d.value.allocated = (void*) function;

	//declare the function, with or without implementation
	symbolTable->declare(functionName, d);
}

void executeReturnStatement(ReturnStatementNode* node) {
	bool* returnFlag = node->returnFlag;
	ParseData* returnValue = node->returnValue;
	*returnValue = node->expression->evaluate();
	*returnFlag = true;
}
