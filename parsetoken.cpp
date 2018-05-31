#include "token.h"
#include "parsetoken.h"

//returns corresponding ParseDataType enum from type TokenType enum
ParseDataType typeTokenConversion(TokenType tt) {
  switch(tt) {
    case INT8: return INT8_T;
    case INT16: return INT16_T;
    case INT32: return INT32_T;
    case INT64: return INT64_T;
    case UINT8: return UINT8_T;
    case UINT16: return UINT16_T;
    case UINT32: return UINT32_T;
    case UINT64: return UINT64_T;
    case CHAR: return CHAR_T;
    case BOOL: return BOOL_T;
    case DOUBLE: return DOUBLE_T;
    case STRING: return STRING_T;
    default: return INVALID_T;
  }
}

//returns corresponding ParseDataType enum from unary operator TokenType enum
ParseOperatorType unaryTokenConversion(TokenType tt) {
  switch(tt) {
    case INC: return INC_OP;
    case DEC: return DEC_OP;
    case ADD: return POSITIVE_OP;
    case SUBTRACT: return NEGATIVE_OP;
    case NOT: return NOT_OP;
    case BIT_NOT: return BIT_NOT_OP;
    default: return INVALID_OP;    
  }
}

//returns corresponding ParseDataType enum from binary operator TokenType enum
ParseOperatorType binaryTokenConversion(TokenType tt) {

  switch(tt) {

    case EXPONENT: return EXPONENT_OP; 
    case MULTIPLY: return MULTIPLY_OP;
    case DIVIDE: return DIVIDE_OP; 
    case MOD: return MOD_OP;
    case ADD: return ADD_OP;
    case SUBTRACT: return SUBTRACT_OP;
 
    case BIT_LEFT: return BIT_LEFT_OP;
    case BIT_RIGHT: return BIT_RIGHT_OP;
    
    case GREATER: return GREATER_OP; 
    case LESS: return LESS_OP; 
    case GREATER_EQ: return GREATER_EQ_OP; 
    case LESS_EQ: return LESS_EQ_OP;
    case EQ_EQ: return EQ_EQ_OP; 
    case NOT_EQ: return NOT_EQ_OP;

    case BIT_AND: return BIT_AND_OP;
    case BIT_XOR: return BIT_XOR_OP;
    case BIT_OR: return BIT_OR_OP;  

    case AND: return AND_OP;
    case XOR: return XOR_OP;
    case OR: return OR_OP;

    case EQ: return EQ_OP; 
    case ADD_EQ: return ADD_EQ_OP;
    case SUBTRACT_EQ: return SUBTRACT_EQ_OP;
    case EXPONENT_EQ: return EXPONENT_EQ_OP;
    case MULTIPLY_EQ: return MULTIPLY_EQ_OP;
    case DIVIDE_EQ: return DIVIDE_EQ_OP;
    case MOD_EQ: return MOD_EQ_OP; 
    case AND_EQ: return AND_EQ_OP;
    case XOR_EQ: return XOR_EQ_OP;
    case OR_EQ: return OR_EQ_OP;
    case BIT_LEFT_EQ: return BIT_LEFT_EQ_OP;
    case BIT_RIGHT_EQ: return BIT_RIGHT_EQ_OP;

    default: return INVALID_OP;
  }  
}


