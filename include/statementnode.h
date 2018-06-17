#ifndef STATEMENTNODE_H
#define STATEMENTNODE_H

#include <vector>
#include "parsetoken.h"
#include "parsenode.h"
#include "symboltable.h"

///////////////////////////////////
///////     Superclass      ///////
///////////////////////////////////

//abstract class that represents statement in tree
class AbstractStatementNode {
  
  public:
    SymbolTable* symbolTable;
    virtual void execute() = 0;
  
};

///////////////////////////////////
///////     Subclasses      ///////
///////////////////////////////////

//represents a single-expression statement
class ExpressionStatementNode : public AbstractStatementNode {

  public:
    AbstractExpressionNode* expression;
    ExpressionStatementNode(AbstractExpressionNode* exp, SymbolTable* symbolTable);
    void execute();
};

//represents print statement that does not append new line character
class PrintStatementNode : public AbstractStatementNode {

  public:
    AbstractExpressionNode* expression;
    PrintStatementNode(AbstractExpressionNode* exp, SymbolTable* symbolTable);
    void execute();
};

//represents print statement that does append new line character
class PrintLineStatementNode : public AbstractStatementNode {

  public:
    AbstractExpressionNode* expression;
    PrintLineStatementNode(AbstractExpressionNode* exp, SymbolTable* symbolTable);
    void execute();
};


//represents a group of statements in braces
class GroupedStatementNode : public AbstractStatementNode {

  public:
    std::vector<AbstractStatementNode*>* statements;
    
    GroupedStatementNode(std::vector<AbstractStatementNode*>* s, SymbolTable* symbolTable);
    void execute();
  
};

//represents if-elif-else structure
class ConditionalStatementNode : public AbstractStatementNode {
  
  public:
    std::vector<AbstractExpressionNode*>* conditions;
    std::vector<AbstractStatementNode*>* statements;
    
    ConditionalStatementNode(std::vector<AbstractExpressionNode*>* cond, std::vector<AbstractStatementNode*>* stat, SymbolTable* symbolTable);
    void execute();
};

//represents declaration (and maybe assignment) of a new variable
class NewAssignmentStatementNode : public AbstractStatementNode {
	
	public:
		char* variable;
		ParseDataType type;
		AbstractExpressionNode* value;
		
		NewAssignmentStatementNode(char* var, ParseDataType typ, AbstractExpressionNode* val, SymbolTable* symbolTable);
		NewAssignmentStatementNode(char* var, ParseDataType typ, SymbolTable* symbolTable);
		void execute();
};


//represents existing variable assignment
class AssignmentStatementNode : public AbstractStatementNode {

  public:
    char* variable;
    AbstractExpressionNode* value;
    
    AssignmentStatementNode(char* var, AbstractExpressionNode* val, SymbolTable* symbolTable);
    void execute();
};



#endif