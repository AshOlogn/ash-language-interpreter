#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include "token.h"
#include "parsetoken.h"
#include "statementnode.h"

//forward declarations to break cyclic dependencies with statementnode.h and parsenode.h
class AbstractStatementNode;

//holds function information
struct Function {
	uint32_t numArgs;
	ParseDataType* argTypes;
	char** argNames;
	std::vector<AbstractStatementNode*>* body;
	ParseDataType returnType;

	//used by return statement to indicate function termination
	//and place the return value somewhere
	ParseData* returnValue;
	bool* returnFlag;
};

#endif