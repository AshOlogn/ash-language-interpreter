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
class AbstractBinaryOperatorNode : public AbstractOperatorNode {
  public:
    AbstractExpressionNode* rightArg;
    virtual ParseData evaluate() = 0;
};

//class that represents arithmetic
class ArithmeticOperatorNode : public AbstractBinaryOperatorNode {
  public:
    ArithmeticOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r);
    ParseData evaluate();
};

//class that represents bitwise and logical operations
class BitLogicalOperatorNode : public AbstractBinaryOperatorNode {
  public:
    BitLogicalOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r);
    ParseData evaluate();
};

//class that represents assignment
class AssignmentOperatorNode : public AbstractBinaryOperatorNode {
  public:
    AssignmentOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r);
    ParseData evaluate();
};

//class that represents comparison
class ComparisonOperatorNode : public AbstractBinaryOperatorNode {
  public:
    ComparisonOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r);
    ParseData evaluate();
};


///////////////////////////////////
///////      Literal        ///////
///////////////////////////////////

//class that holds literal data found in the source code
class LiteralNode : public AbstractExpressionNode {
  public:
    ParseData data;
    LiteralNode(ParseData d);
    ParseData evaluate();
};


///////////////////////////////////
///////      Grouped        ///////
///////////////////////////////////

//class that holds parenthesized expression
class GroupedExpressionNode : public AbstractExpressionNode {
  public:
    AbstractExpressionNode* closedExpression;
    GroupedExpressionNode(AbstractExpressionNode* closedExp);
    ParseData evaluate();
};


#endif
