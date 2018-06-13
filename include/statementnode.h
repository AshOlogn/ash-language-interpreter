#ifndef STATEMENTNODE_H
#define STATEMENTNODE_H

#include <vector>
#include "parsetoken.h"
#include "parsenode.h"

///////////////////////////////////
///////     Superclass      ///////
///////////////////////////////////

//abstract class that represents statement in tree
class AbstractStatementNode {
  
  public:
    virtual void execute() = 0;
  
};

///////////////////////////////////
///////     Subclasses      ///////
///////////////////////////////////

//represents a single-expression statement
class ExpressionStatementNode : public AbstractStatementNode {

  public:
    AbstractExpressionNode* expression;
    ExpressionStatementNode(AbstractExpressionNode* exp);
    void execute();
};

//represents print statement that does not append new line character
class PrintStatementNode : public AbstractStatementNode {

  public:
    AbstractExpressionNode* expression;
    PrintStatementNode(AbstractExpressionNode* exp);
    void execute();
};

//represents print statement that does append new line character
class PrintLineStatementNode : public AbstractStatementNode {

  public:
    AbstractExpressionNode* expression;
    PrintLineStatementNode(AbstractExpressionNode* exp);
    void execute();
};


//represents a group of statements in braces
class GroupedStatementNode : public AbstractStatementNode {

  public:
    std::vector<AbstractStatementNode*>* statements;
    
    GroupedStatementNode(std::vector<AbstractStatementNode*>* s);
    void execute();
  
};

//represents if-elif-else structure
class ConditionalStatementNode : public AbstractStatementNode {
  
  public:
    std::vector<AbstractExpressionNode*>* conditions;
    std::vector<AbstractStatementNode*>* statements;
    
    ConditionalStatementNode(std::vector<AbstractExpressionNode*>* cond, std::vector<AbstractStatementNode*>* stat);
    void execute();
};




#endif