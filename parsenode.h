#ifndef PARSENODE_H
#define PARSENODE_H

#include "parsetoken.h"

//abstract class that represents node in AST
class AbstractExpressionNode {
  public:
    virtual ParseData evaluate() = 0;
};


///////////////////////////////////
///////     Operations      ///////
///////////////////////////////////

//abstract class that represents an operation
class AbstractOperatorNode : public AbstractExpressionNode {
  public:
    ParseOperatorType operation;
    AbstractExpressionNode* leftArg;
    virtual ParseData evaluate() = 0;
};

//class that represents unary operations
class UnaryOperatorNode : public AbstractOperatorNode {
  public:
    UnaryOperatorNode(ParseOperatorType op, AbstractExpressionNode* l);
    ParseData evaluate();
};


//class that represents binary operations
class BinaryOperatorNode : public AbstractOperatorNode {
  public:
    AbstractExpressionNode* rightArg;
    ParseData evaluate();
};


///////////////////////////////////
///////      Literal        ///////
///////////////////////////////////

//class that holds literal data found in the source code
class LiteralNode : public AbstractExpressionNode {
  public:
    ParseData data;
    ParseData evaluate();
};


///////////////////////////////////
///////      Grouped        ///////
///////////////////////////////////

//class that holds parenthesized expression
class GroupedExpressionNode : public AbstractExpressionNode {
  public:
    AbstractExpressionNode* closedExpression;
    ParseData evaluate();
};


#endif
