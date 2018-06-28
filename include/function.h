#ifndef FUNCTION_H
#define FUNCTION_H

#include "token.h"
#include "parsetoken.h"
#include "statementnode.h"

//holds function information
struct Function {
	uint32_t numArgs;
	ParseDataType* argTypes;
	char** argNames;
	GroupedStatementNode* body;
	ParseDataType returnType;
};

#endif