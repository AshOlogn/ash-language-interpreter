#ifndef CLASS_H
#define CLASS_H

#include <vector>
#include "symboltable.h"
#include "statementnode.h"

class AbstractStatementNode;
class FunctionStatementNode;

//represents class in this  language
struct Class {

	//superclass in inheritance hierarchy
	char* superClass;

	//list of instance field types (could be ParseDataType or char* representing class)
	std::vector<void*>* instanceTypes;

	//list of instance names
	std::vector<char*>* instanceNames;

	//list of method declarations
	std::vector<FunctionStatementNode*>* methods;
};

struct Instance {

	//basic identifier information
	char* className;
	char* superClassName;

	//symbol table of member values
	SymbolTable* symbolTable;
};

#endif