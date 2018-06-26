#ifndef PARSETOKEN_H
#define PARSETOKEN_H

#include "token.h"


////////////////////////////////////
///////      Structures      ///////
////////////////////////////////////

//represents data type
enum ParseDataType {

  INT8_T, INT16_T, INT32_T, INT64_T,
  UINT8_T, UINT16_T, UINT32_T, UINT64_T,
  CHAR_T, BOOL_T, DOUBLE_T, STRING_T,
	VOID_T,
  CUSTOM_T,
  INVALID_T

};

//represents operator type
enum ParseOperatorType {

  POSTFIX_INC_OP, POSTFIX_DEC_OP, PREFIX_INC_OP, PREFIX_DEC_OP, PARENTHESIZE_OP, 
  CAST_OP, MEMBER_ACCESS_OP, POSITIVE_OP, NEGATIVE_OP, NOT_OP, BIT_NOT_OP,

  EXPONENT_OP, 
  MULTIPLY_OP, DIVIDE_OP, MOD_OP,
  ADD_OP, SUBTRACT_OP,

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
  MOD_EQ_OP, AND_EQ_OP, XOR_EQ_OP, OR_EQ_OP, BIT_LEFT_EQ_OP, BIT_RIGHT_EQ_OP,

  INVALID_OP
};

//holds type and data information
struct ParseData {
  ParseDataType type;
  Data value;
};

////////////////////////////////////
///////       To String      ///////
////////////////////////////////////

const char* toStringParseDataType(ParseDataType p);
const char* toStringParseOperatorType(ParseOperatorType p);
const char* toWordParseOperatorType(ParseOperatorType p);
char* toStringParseData(ParseData d);


////////////////////////////////////
///////      Useful Info     ///////
////////////////////////////////////

bool isNumberParseDataType(ParseDataType p);
bool isIntParseDataType(ParseDataType p);
bool isSignedIntParseDataType(ParseDataType p);
bool isUnsignedIntParseDataType(ParseDataType p);

ParseDataType getLargerNumberParseDataType(ParseDataType p1, ParseDataType p2);

bool isBitwiseParseOperatorType(ParseOperatorType p);
bool isLogicalParseOperatorType(ParseOperatorType p);


////////////////////////////////////
///////        Convert       ///////
////////////////////////////////////

//returns corresponding ParseDataType enum from type TokenType enum
ParseDataType typeTokenConversion(TokenType tt);

//returns corresponding ParseDataType enum from unary operator TokenType enum
ParseOperatorType unaryTokenConversion(TokenType tt);

//returns corresponding ParseDataType enum from binary operator TokenType enum
ParseOperatorType binaryTokenConversion(TokenType tt);

#endif

