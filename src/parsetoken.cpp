#include <cstring>
#include <string>
#include <iostream>
#include "token.h"
#include "parsetoken.h"

static const ParseDataType signedIntegerTypes[] = {INT8_T, INT16_T, INT32_T, INT64_T}; 
static const ParseDataType unsignedIntegerTypes[] = {UINT8_T, UINT16_T, UINT32_T, UINT64_T}; 


////////////////////////////////////
///////      Useful Info     ///////
////////////////////////////////////

bool isNumberParseDataType(ParseDataType p) {
  
  return p == INT8_T ||  p == INT16_T || p == INT32_T || p == INT64_T ||
         p == UINT8_T || p == UINT16_T || p == UINT32_T || p == UINT64_T ||
         p == CHAR_T || p == DOUBLE_T;
}

bool isIntParseDataType(ParseDataType p) {
  
  return p == INT8_T ||  p == INT16_T || p == INT32_T || p == INT64_T ||
         p == UINT8_T || p == UINT16_T || p == UINT32_T || p == UINT64_T ||
         p == CHAR_T;
}

bool isSignedIntParseDataType(ParseDataType p) {
  
  return p == INT8_T ||  p == INT16_T || p == INT32_T || p == INT64_T;
}

bool isUnsignedIntParseDataType(ParseDataType p) {

  return p == UINT8_T || p == UINT16_T || p == UINT32_T || p == UINT64_T ||
         p == CHAR_T;  
}


ParseDataType getLargerNumberParseDataType(ParseDataType p1, ParseDataType p2) {
 
  //some special cases
  if(p1 == DOUBLE_T || p2 == DOUBLE_T)
    return DOUBLE_T;

  //CHAR_T only returned if BOTH types are CHAR_T already (otherwise it's int)
  if(p1 == CHAR_T && p2 == CHAR_T)
    return CHAR_T;

  //pre-process
  ParseDataType P1 = (p1 == CHAR_T) ? UINT8_T : p1;
  ParseDataType P2 = (p2 == CHAR_T) ? UINT8_T : p2;

  //find the larger type
  uint8_t i1 = 0;
  uint8_t i2 = 0;

  while((signedIntegerTypes[i1] != P1) &&  (unsignedIntegerTypes[i1] != P1))
    i1++;

  while((signedIntegerTypes[i2] != P2) &&  (unsignedIntegerTypes[i2] != P2))
    i2++;

  uint8_t max = (i1 > i2) ? i1 : i2;
  return (isUnsignedIntParseDataType(P1) && isUnsignedIntParseDataType(P2)) ?
          unsignedIntegerTypes[max] : signedIntegerTypes[max];  

}


bool isBitwiseParseOperatorType(ParseOperatorType p) {

  return p == BIT_AND_OP || p == BIT_XOR_OP || p == BIT_OR_OP || 
         p == BIT_NOT_OP || p == BIT_LEFT_OP || p == BIT_RIGHT_OP;
}

bool isLogicalParseOperatorType(ParseOperatorType p) {
  
  return p == AND_OP || p == XOR_OP || p == OR_OP || p == NOT_OP;
}


////////////////////////////////////
///////        Convert       ///////
////////////////////////////////////

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



////////////////////////////////////
///////       To String      ///////
////////////////////////////////////

//returns string representation of ParseDataType enum
const char* toStringParseDataType(ParseDataType p) {
 
  switch(p) {
  
    case INT8_T: return "INT8_T";
    case INT16_T: return "INT16_T";
    case INT32_T: return "INT64_T";
    case UINT8_T: return "UINT8_T"; 
    case UINT16_T: return "UINT32_T"; 
    case UINT64_T: return "UINT64_T";
    case CHAR_T: return "CHAR_T";
    case BOOL_T: return "BOOL_T"; 
    case DOUBLE_T: return "DOUBLE_T"; 
    case STRING_T: return "STRING_T";
    case CUSTOM_T: return "CUSTOM_T";
    default: return "INVALID_T";
  } 
}

//returns string representation of ParseOperatorType enum
const char* toStringParseOperatorType(ParseOperatorType p) {

  switch(p) {

    case INC_OP: return "++";
    case DEC_OP: return "--";
    case POSITIVE_OP: return "+";
    case NEGATIVE_OP: return "-";
    case ADD_OP: return "+";
    case SUBTRACT_OP: return "-";
    case NOT_OP: return "!";
    case BIT_NOT_OP: return "~";

    case EXPONENT_OP: return "**"; 
    case MULTIPLY_OP: return "*";
    case DIVIDE_OP: return "/"; 
    case MOD_OP: return "%";
 
    case BIT_LEFT_OP: return "<<";
    case BIT_RIGHT_OP: return ">>";
    
    case GREATER_OP: return ">"; 
    case LESS_OP: return "<"; 
    case GREATER_EQ_OP: return ">="; 
    case LESS_EQ_OP: return "<=";
    case EQ_EQ_OP: return "=="; 
    case NOT_EQ_OP: return "!=";

    case BIT_AND_OP: return "&";
    case BIT_XOR_OP: return "^";
    case BIT_OR_OP: return "|";  

    case AND_OP: return "&&";
    case XOR_OP: return "^^";
    case OR_OP: return "||";

    case EQ_OP: return "="; 
    case ADD_EQ_OP: return "+=";
    case SUBTRACT_EQ_OP: return "-=";
    case EXPONENT_EQ_OP: return "**=";
    case MULTIPLY_EQ_OP: return "*=";
    case DIVIDE_EQ_OP: return "/=";
    case MOD_EQ_OP: return "%="; 
    case AND_EQ_OP: return "&=";
    case XOR_EQ_OP: return "^=";
    case OR_EQ_OP: return "|=";
    case BIT_LEFT_EQ_OP: return "<<=";
    case BIT_RIGHT_EQ_OP: return ">>=";
    
    default: return "INVALID";
  }
}


char* toStringParseData(ParseData d) {

  switch(d.type) {
  
    case INT8_T: {
      int8_t val = (int8_t) d.value.integer;
      std::string str = std::to_string(val);
      char* c = new char[str.length()+1];
      std::strcpy(c, str.c_str());
      return c;
    }

    case INT16_T: {
      int16_t val = (int16_t) d.value.integer;  
      std::string str = std::to_string(val);
      char* c = new char[str.length()+1];
      std::strcpy(c, str.c_str());
      return c;
    }

    case INT32_T: {
      int32_t val = (int32_t) d.value.integer;
      std::string str = std::to_string(val);
      char* c = new char[str.length()+1];
      std::strcpy(c, str.c_str());
      return c;
    }

    case INT64_T: {
      int64_t val = (int64_t) d.value.integer;
      std::string str = std::to_string(val);
      char* c = new char[str.length()+1];
      std::strcpy(c, str.c_str());
      return c;
    }

    case UINT8_T: {
      uint8_t val = (uint8_t) d.value.integer;
      std::string str = std::to_string(val);
      char* c = new char[str.length()+1];
      std::strcpy(c, str.c_str());
      return c;
    }

    case UINT16_T: {
      uint16_t val = (uint16_t) d.value.integer;  
      std::string str = std::to_string(val);
      char* c = new char[str.length()+1];
      std::strcpy(c, str.c_str());
      return c;
    }

    case UINT32_T: {
      uint32_t val = (uint32_t) d.value.integer;
      std::string str = std::to_string(val);
      char* c = new char[str.length()+1];
      std::strcpy(c, str.c_str());
      return c;
    }

    case UINT64_T: {
      uint64_t val = (uint64_t) d.value.integer;
      std::string str = std::to_string(val);
      char* c = new char[str.length()+1];
      std::strcpy(c, str.c_str());
      return c;
    }

    case CHAR_T: {
      char val = (char) ((uint8_t) d.value.integer);
      char* c = new char[2];
      c[0] = val;
      c[1] = '\0';
      return c; 
    }

    case DOUBLE_T: {
      std::string str = std::to_string((double) d.value.floatingPoint);
      char* c = new char[str.length()+1];
      std::strcpy(c, str.c_str());
      return c;
    }
    
    case BOOL_T: {
     
      if(d.value.integer) {
        char* ans = new char[5];
        std::strcpy(ans, "true");
        return ans;
      } else {
        char* ans = new char[6];
        std::strcpy(ans, "false");
        return ans;
      }
    }

    case STRING_T: {
      char* val = (char*) d.value.allocated;
      char* c = new char[strlen(val)+1];
      std::strcpy(c, val);
      return c;
    }

    default: {
      char* c = new char[8];
      std::strcpy(c, "INVALID");
      return c;
    }

  }

  
}


