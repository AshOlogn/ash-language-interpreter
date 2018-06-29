#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <cstring>
#include "token.h"
#include "typehandler.h"
#include "functioneval.h"
#include "symboltable.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "statementnode.h"
#include "function.h"

using namespace std;

ParseData evaluateFunctionExpression(FunctionExpressionNode* node) {
	
	SymbolTable* symbolTable = node->symbolTable;

	//function information
	Function* function = node->function;
	uint32_t numArgs = node->numArgs;
	AbstractExpressionNode** args = node->arguments;
	char** argNames = function->argNames;
	vector<AbstractStatementNode*>* body = function->body;
	
	
	//evaluate parameters
	ParseData* arguments = (ParseData*) malloc(sizeof(ParseData) * numArgs);
	for(uint32_t i = 0; i < numArgs; i++) {
		arguments[i] = args[i]->evaluate();
	}

	//enter function scope
	symbolTable->enterNewScope();

	//declare parameters with computed arguments
	for(uint32_t i = 0; i < numArgs; i++) {
		symbolTable->declare(argNames[i], arguments[i]);
	}

	//execute statements in the body
	vector<AbstractStatementNode*>::iterator it;
	bool* returnFlag = node->isReturned;
	*returnFlag = false;
	for(it = body->begin(); !(*returnFlag) && it != body->end(); it++) {
		(*it)->execute();
	}

	//return value placed in correct address by return statement
	symbolTable->leaveScope();
	return *(node->returnValue);
}