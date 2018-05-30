#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

//represents data type
enum ParseDataType {
  INT8_T, INT16_T, INT32_T, INT64_T,
  CHAR_T, BOOL_T, DOUBLE_T, STRING_T,
  CUSTOM_T
};

//represents operator type
enum ParseOperatorType {
  INC_OP, DEC_OP, LEFT_PAREN_OP, RIGHT_PAREN_OP, LEFT_BRACKET_OP, RIGHT_BRACKET_OP,
  PERIOD_OP, ADD_OP, SUBTRACT_OP, NOT_OP, BIT_NOT_OP,

  EXPONENT_OP, 
  MULTIPLY_OP, DIVIDE_OP, MOD_OP,
  BIT_LEFT_OP, BIT_RIGHT_OP,

  GREATER_OP, LESS_OP, GREATER_EQ_OP, LESS_EQ_OP,
  EQ_EQ_OP, NOT_EQ_OP,

  BIT_AND_OP,
  BIT_XOR_OP,
  BIT_OR_OP,  

  AND_OP,
  XOR_OP,
  OR_OP,

  EQ_OP, ADD_EQ_OP, SUBTRACT_EQ_OP, EXPONENT_EQ_OP, MULTIPLY_EQ_OP, DIVIDE_EQ_OP, 
  MOD_EQ_OP, AND_EQ_OP, XOR_EQ_OP, OR_EQ_OP, BIT_LEFT_EQ_OP, BIT_RIGHT_EQ_OP
};

//holds type and data information
struct ParseData {
  ParseDataType type;
  Data value;
};

//method that resolves types
ParseData* resolveTypes(ParseOperatorType operation, ParseData arg1, ParseData arg2);


//abstract class that represents node in AST
class AbstractParseNode {
  public:
    virtual ParseData evaluate() = 0;
};

//abstract class that represents operation
class AbstractOperatorNode : public AbstractParseNode {
  public:
    ParseOperatorType operation;
    virtual ParseData evaluate() = 0;
};

//class that holds literal data found in the source code
class LiteralNode : public AbstractParseNode {
  public:
    ParseData data;
    ParseData evaluate();
};


#endif
