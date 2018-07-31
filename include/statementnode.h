#ifndef STATEMENTNODE_H
#define STATEMENTNODE_H

#include <vector>
#include <string>
#include <cstdint>
#include "parsetoken.h"
#include "parsenode.h"
#include "symboltable.h"
#include "function.h"

//forward declarations to break cyclic dependencies with function.h and parsenode.h
struct Function;
class AbstractExpressionNode;

///////////////////////////////////
///////     Superclass      ///////
///////////////////////////////////

//abstract class that represents statement in tree
class AbstractStatementNode {
  
  public:
		uint32_t startLine;
		uint32_t endLine;
    SymbolTable* symbolTable;
		SymbolTable* classSymbolTable;
    virtual void execute() = 0;
  
};

///////////////////////////////////
///////     Subclasses      ///////
///////////////////////////////////

//represents a single-expression statement
class ExpressionStatementNode : public AbstractStatementNode {

  public:
    AbstractExpressionNode* expression;
    ExpressionStatementNode(AbstractExpressionNode* exp, SymbolTable* symbolTable, SymbolTable* classSymbolTable);
    void execute();
};

//represents print statement that does not append new line character
class PrintStatementNode : public AbstractStatementNode {

  public:
    AbstractExpressionNode* expression;
    PrintStatementNode(AbstractExpressionNode* exp, SymbolTable* symbolTable, SymbolTable* classSymbolTable, uint32_t startLine);
    void execute();
};

//represents print statement that does append new line character
class PrintLineStatementNode : public AbstractStatementNode {

  public:
    AbstractExpressionNode* expression;
    PrintLineStatementNode(AbstractExpressionNode* exp, SymbolTable* symbolTable, SymbolTable* classSymbolTable, uint32_t startLine);
    void execute();
};


//represents a group of statements in braces
class GroupedStatementNode : public AbstractStatementNode {

  public:
    std::vector<AbstractStatementNode*>* statements;

    GroupedStatementNode(std::vector<AbstractStatementNode*>* s, SymbolTable* symbolTable, SymbolTable* classSymbolTable, uint32_t startLine, uint32_t endLine);
    void execute();
  
};

//represents if-elif-else structure
class ConditionalStatementNode : public AbstractStatementNode {
  
  public:
    std::vector<AbstractExpressionNode*>* conditions;
    std::vector<AbstractStatementNode*>* statements;
    
    ConditionalStatementNode(std::vector<AbstractExpressionNode*>* cond, std::vector<AbstractStatementNode*>* stat, SymbolTable* symbolTable, SymbolTable* classSymbolTable, uint32_t startLine, uint32_t endLine);
    void execute();
};


//represents while loop
class WhileStatementNode: public AbstractStatementNode {

  public:
    AbstractExpressionNode* condition;
    AbstractStatementNode* body;
    SymbolTable* symbolTable;
    
    WhileStatementNode(AbstractExpressionNode* cond, AbstractStatementNode* bod, SymbolTable* symbolTable, SymbolTable* classSymbolTable, uint32_t startLine);
    void execute();
};

//represents for loop
class ForStatementNode: public AbstractStatementNode {

  public:
    AbstractStatementNode* initialization;
    AbstractStatementNode* update;
    AbstractStatementNode* body;
    AbstractExpressionNode* condition;
    SymbolTable* symbolTable;
    
    ForStatementNode(AbstractStatementNode* init, AbstractStatementNode* upd, AbstractStatementNode* bod, AbstractExpressionNode* cond, SymbolTable* symbolTable, SymbolTable* classSymbolTable, uint32_t startLine);
    void execute();
};

//represents declaration (and maybe assignment) of a new variable
class NewAssignmentStatementNode : public AbstractStatementNode {
	
	public:
		std::string variable;
		ParseDataType type;
		AbstractExpressionNode* value;
		
		NewAssignmentStatementNode(std::string var, ParseDataType typ, AbstractExpressionNode* val, SymbolTable* symbolTable, SymbolTable* classSymbolTable, uint32_t startLine);
		NewAssignmentStatementNode(std::string var, ParseDataType typ, SymbolTable* symbolTable, SymbolTable* classSymbolTable, uint32_t startLine, uint32_t endLine);
		void execute();
};


//represents existing variable assignment
class AssignmentStatementNode : public AbstractStatementNode {

  public:
    std::string variable;
    AbstractExpressionNode* value;
    
    AssignmentStatementNode(std::string var, AbstractExpressionNode* val, SymbolTable* symbolTable, SymbolTable* classSymbolTable, uint32_t startLine);
    void execute();
};

//represents array index assignment, like arr[i] = 5
class ArrayAssignmentStatementNode : public AbstractStatementNode {

	public:
		std::string variable;
		AbstractExpressionNode* index;
		AbstractExpressionNode* value;

		ArrayAssignmentStatementNode(std::string variable, AbstractExpressionNode* index, AbstractExpressionNode* value, SymbolTable* symbolTable, SymbolTable* classSymbolTable, uint32_t startLine);
		void execute();
};

//represents return statement in a function
class ReturnStatementNode : public AbstractStatementNode {
	
	public:
		bool* returnFlag;
		ParseData* returnValue;
		AbstractExpressionNode* expression;

		ReturnStatementNode(AbstractExpressionNode* expression, bool* returnFlag, ParseData* returnValue, uint32_t startLine);
		void execute();
};

//represents declaration (and maybe definition) of a function
class FunctionStatementNode : public AbstractStatementNode {

	public:
		Function* function;
		std::string functionName;

		FunctionStatementNode(std::string functionName, Function* function, SymbolTable* symbolTable, SymbolTable* classSymbolTable);
		FunctionStatementNode(std::string functionName, SymbolTable* symbolTable, SymbolTable* classSymbolTable);
		void execute();
};

#endif