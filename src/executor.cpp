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
#include "exceptions.h"

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
	ParseDataType subType = node->subType;
  
  //add variable to table
  if(node->value != NULL) {

    ParseData d = node->value->evaluate();
				
		if(type == ARRAY_T) {

			//if array, change array value's subtype to fit variable's subtype
			Array* arr = (Array*) d.value.allocated;
			arr->subtype = subType;
			symbolTable->declare(variable, d);

		} else {
			//consider implicit casting
    	symbolTable->declare(variable, castHelper(d, type));
		}

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

	if(type == ARRAY_T) {

		//deep copy of array, casting as necessary
		Array* arr = (Array*) (symbolTable->get(variable)).value.allocated;
		ParseDataType subType = arr->subtype;

		Array* origArr = (Array*) d.value.allocated;
		uint32_t length = arr->length = origArr->length;
		
		ParseData* values = new ParseData[length];
		ParseData* origValues = origArr->values;

		for(uint32_t i = 0; i < length; i++) {
			values[i] = castHelper(origValues[i], subType);
		}

		arr->values = values;

	} else {
		//update variable value (innermost scope)
  	symbolTable->update(variable, castHelper(d, type));
	}
}

void executeArrayAssignmentStatement(ArrayAssignmentStatementNode* node) {

	//get stuff out of the node first
	SymbolTable* symbolTable = node->symbolTable;
	std::string variable = node->variable;
	int32_t index = (int32_t) node->index->evaluate().value.integer;

	if(node->isArray) {

		//get array from the symbol table
		Array* array = (Array*) (symbolTable->get(variable)).value.allocated;
		int32_t length = (int32_t) array->length;

		//if negative index
		int32_t finalIndex = (index < 0) ? index + length : index;

		//if out of bounds, throw an exception
		if(finalIndex < 0 || finalIndex > length-1)
			throw OutOfBoundsException(true, length, index, node->context, node->startLine, node->endLine);

		ParseData value = node->value->evaluate();

		//do the assignment
		array->values[finalIndex] = value;

	} else {

		char* str = (char*) (symbolTable->get(variable)).value.allocated;
		int32_t length = (int32_t) strlen(str);

		//if negative index
		int32_t finalIndex = (index < 0) ? index + length : index;

		//if out of bounds, throw an exception
		if(finalIndex < 0 || finalIndex > length-1)
			throw OutOfBoundsException(false, length, index, node->context, node->startLine, node->endLine);

		ParseData value = node->value->evaluate();

		//do the assignment
		str[finalIndex] = (unsigned char) value.value.integer;
	}

	
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
