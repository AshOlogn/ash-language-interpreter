#ifndef CLASS_H
#define CLASS_H

#include <vector>
#include "symboltable.h"
#include "statementnode.h"

class AbstractStatementNode;

//represents class in this  language
struct Class {
	//superclass in inheritance hierarchy
	char* superClass;

	//statements declaring instance fields and methods
	std::vector<AbstractStatementNode*>* body;

	//symbol table containing all instance fields and methods
	SymbolTable* symbolTable;

	//symbol table containing all class information in this program
	SymbolTable* classSymbolTable;
};

#endif