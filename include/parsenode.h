#ifndef PARSENODE_H
#define PARSENODE_H
#include <cstdint>
#include <string>
#include "parsetoken.h"
#include "symboltable.h"
#include "function.h"

//forward declarations to break cyclic dependencies with function.h and statementnode.h
struct Function;
class AbstractStatementNode;

///////////////////////////////////
///////     Superclass      ///////
///////////////////////////////////

//abstract class that represents node in AST
class AbstractExpressionNode {
  public:
    uint32_t startLine;
    uint32_t endLine;
    ParseDataType evalType;
		ParseDataType subType;
    virtual ParseData evaluate() = 0;
    virtual std::string toString() = 0;
};

///////////////////////////////////
///////     Assignment      ///////
///////////////////////////////////
class AssignmentExpressionNode : public AbstractExpressionNode {
  
  public:
    std::string variable;
    ParseDataType variableType;
    AbstractExpressionNode* value;
    SymbolTable* symbolTable;
    
    AssignmentExpressionNode(std::string variable, ParseDataType variableType, AbstractExpressionNode* value, SymbolTable* symbolTable, uint32_t varLine);
    ParseData evaluate();
    std::string toString();
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
    virtual std::string toString() = 0;
};

//class that represents unary operations
class UnaryOperatorNode : public AbstractOperatorNode {
  public:
    SymbolTable* symbolTable;
    UnaryOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, uint32_t operatorLine);
    UnaryOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, SymbolTable* symbolTable, uint32_t operatorLine);
    ParseData evaluate();
    std::string toString();
};

//class that represents binary operations
class AbstractBinaryOperatorNode : public AbstractOperatorNode {
  public:
    AbstractExpressionNode* rightArg;
    virtual ParseData evaluate() = 0;
    std::string toString();
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

//class that represents comparison
class ComparisonOperatorNode : public AbstractBinaryOperatorNode {
  public:
    ComparisonOperatorNode(ParseOperatorType op, AbstractExpressionNode* l, AbstractExpressionNode* r);
    ParseData evaluate();
};


///////////////////////////////////
///////        Cast         ///////
///////////////////////////////////

//class that holds literal data found in the source code
class CastNode : public AbstractExpressionNode {
  public:
    AbstractExpressionNode* expression;
    ParseDataType finalType;
    
    CastNode(AbstractExpressionNode* e, ParseDataType type, uint32_t startLine);
    ParseData evaluate();
    std::string toString();
};

///////////////////////////////////
///////    Member Access    ///////
///////////////////////////////////
class ArrayAccessNode : public AbstractExpressionNode {
  
  public:
    AbstractExpressionNode* array;
    AbstractExpressionNode* start;
    AbstractExpressionNode* end;
    bool isSlice;
    
    ArrayAccessNode(AbstractExpressionNode* arr, AbstractExpressionNode* s, uint32_t endLine);
    ArrayAccessNode(AbstractExpressionNode* arr, AbstractExpressionNode* s, AbstractExpressionNode* e, uint32_t endLine);
  
    ParseData evaluate();
    std::string toString();
};

//represents an array of primitive values
class ArrayNode : public AbstractExpressionNode {

	public:
		bool isInitialized;
		AbstractExpressionNode* length;
		AbstractExpressionNode** values;

		ArrayNode(ParseDataType subType, bool isInitialized, AbstractExpressionNode* length, AbstractExpressionNode** values, uint32_t startLine, uint32_t endLine);
		ParseData evaluate();
		std::string toString();
};

///////////////////////////////////
///////      Variable       ///////
///////////////////////////////////

class VariableNode : public AbstractExpressionNode {
  public:
    SymbolTable* symbolTable;
    std::string variable;
    VariableNode(std::string var, SymbolTable* table, uint32_t line);
    ParseData evaluate();
    std::string toString();
};

///////////////////////////////////
///////    Function Call    ///////
///////////////////////////////////

class FunctionExpressionNode : public AbstractExpressionNode {
	public:
		uint32_t numArgs;
		AbstractExpressionNode** arguments;
		Function* function;
		SymbolTable* symbolTable;
		bool* isReturned;
		ParseData* returnValue;

		FunctionExpressionNode(uint32_t numArgs, AbstractExpressionNode** arguments, Function* function, SymbolTable* symbolTable);	
		ParseData evaluate();
		std::string toString();
};


///////////////////////////////////
///////      Literal        ///////
///////////////////////////////////

//class that holds literal data found in the source code
class LiteralNode : public AbstractExpressionNode {
  public:
    ParseData data;
    LiteralNode(ParseData d, uint32_t line);
    ParseData evaluate();
    std::string toString();
};


///////////////////////////////////
///////      Grouped        ///////
///////////////////////////////////

//class that holds parenthesized expression
class GroupedExpressionNode : public AbstractExpressionNode {
  public:
    AbstractExpressionNode* closedExpression;
    GroupedExpressionNode(AbstractExpressionNode* closedExp, uint32_t startLine, uint32_t endLine);
    ParseData evaluate();
    std::string toString();
};


#endif
