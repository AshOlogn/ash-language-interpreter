#include <cstdint>
#include <cmath>
#include <cstring>
#include <string>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"


//helper functions

//exponentiation
template <typename T1, typename T2>
ParseData exponentHelper(T1 n1, T2 n2) {
 
  //first store the values in appropriate variables
  T1 arg1 = n1;
  T2 arg2 = n2;
  
  ParseData d;
  d.type = DOUBLE_T;
  d.value.floatingPoint = pow((double) arg1, (double) arg2);
 
  return d;
}

//division
template <typename T1, typename T2>
ParseData divisionHelper(T1 n1, T2 n2, ParseDataType type) {
 
  //first store the values in appropriate variables
  T1 arg1 = n1;
  T2 arg2 = n2;
  
  ParseData d;
  d.type = type;
  
  switch(type) {
    case CHAR_T: d.value.integer = (unsigned char) (arg1/arg2); break;
    case INT32_T: d.value.integer = (int32_t) (arg1/arg2); break;
    case INT64_T: d.value.integer = (int64_t) (arg1/arg2); break;
    case UINT32_T: d.value.integer = (uint32_t) (arg1/arg2); break;
    case UINT64_T: d.value.integer = (uint64_t) (arg1/arg2); break;
    case DOUBLE_T: d.value.floatingPoint = (double) (arg1/arg2); break;
  }
 
  return d;
}

//multiplication
template <typename T1, typename T2>
ParseData multiplicationHelper(T1 n1, T2 n2, ParseDataType type) {
 
  //first store the values in appropriate variables
  T1 arg1 = n1;
  T2 arg2 = n2;
  
  ParseData d;
  d.type = type;
  
  switch(type) {
    case CHAR_T: d.value.integer = (unsigned char) (arg1*arg2); break;
    case INT32_T: d.value.integer = (int32_t) (arg1*arg2); break;
    case INT64_T: d.value.integer = (int64_t) (arg1*arg2); break;
    case UINT32_T: d.value.integer = (uint32_t) (arg1*arg2); break;
    case UINT64_T: d.value.integer = (uint64_t) (arg1*arg2); break;
    case DOUBLE_T: d.value.floatingPoint = (double) (arg1*arg2); break;
  }
 
  return d;
}

//modulus (many cases of template resolution to handle floating-point mods)
template <typename T1, typename T2>
ParseData modHelper(T1 n1, T2 n2, ParseDataType type) {
 
  //first store the values in appropriate variables
  T1 arg1 = n1;
  T2 arg2 = n2;
  
  ParseData d;
  d.type = type;
  
  switch(type) {
    case CHAR_T: d.value.integer = (unsigned char) (arg1 % arg2); break;
    case INT32_T: d.value.integer = (int32_t) (arg1 % arg2); break;
    case INT64_T: d.value.integer = (int64_t) (arg1 % arg2); break;
    case UINT32_T: d.value.integer = (uint32_t) (arg1 % arg2); break;
    case UINT64_T: d.value.integer = (uint64_t) (arg1 % arg2); break;
  }
  
  return d;
}

template<>
ParseData modHelper<double, unsigned char>(double n1, unsigned char n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod(n1, (double) n2); return d;
}

template<>
ParseData modHelper<double, int32_t>(double n1, int32_t n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod(n1, (double) n2); return d;
}

template<>
ParseData modHelper<double, int64_t>(double n1, int64_t n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod(n1, (double) n2); return d;
}

template<>
ParseData modHelper<double, uint32_t>(double n1, uint32_t n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod(n1, (double) n2); return d;
}

template<>
ParseData modHelper<double, uint64_t>(double n1, uint64_t n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod(n1, (double) n2); return d;
}

template<>
ParseData modHelper<double, double>(double n1, double n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod(n1, n2); return d;
}

template<>
ParseData modHelper<unsigned char, double>(unsigned char n1, double n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod((double) n1, n2); return d;
}

template<>
ParseData modHelper<int32_t, double>(int32_t n1, double n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod((double) n1, n2); return d;
}

template<>
ParseData modHelper<int64_t, double>(int64_t n1, double n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod((double) n1, n2); return d;
}

template<>
ParseData modHelper<uint32_t, double>(uint32_t n1, double n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod((double) n1, n2); return d;
}

template<>
ParseData modHelper<uint64_t, double>(uint64_t n1, double n2, ParseDataType type) {
  ParseData d; d.type = type; d.value.floatingPoint = fmod((double) n1, n2); return d;
}

//subtraction
template <typename T1, typename T2>
ParseData subtractionHelper(T1 n1, T2 n2, ParseDataType type) {
 
  //first store the values in appropriate variables
  T1 arg1 = n1;
  T2 arg2 = n2;
  
  ParseData d;
  d.type = type;
  
  switch(type) {
    case CHAR_T: d.value.integer = (unsigned char) (arg1-arg2); break;
    case INT32_T: d.value.integer = (int32_t) (arg1-arg2); break;
    case INT64_T: d.value.integer = (int64_t) (arg1-arg2); break;
    case UINT32_T: d.value.integer = (uint32_t) (arg1-arg2); break;
    case UINT64_T: d.value.integer = (uint64_t) (arg1-arg2); break;
    case DOUBLE_T: d.value.floatingPoint = (double) (arg1-arg2); break;
  }
 
  return d;
}

//addition (many cases of template resolution for string concatentation cases)
template <typename T1, typename T2>
ParseData additionHelper(T1 n1, T2 n2, ParseDataType type) {
 
  //first store the values in appropriate variables
  T1 arg1 = n1;
  T2 arg2 = n2;
  
  ParseData d;
  d.type = type;
  
  switch(type) {
    case CHAR_T: d.value.integer = (unsigned char) (arg1+arg2); break;
    case INT32_T: d.value.integer = (int32_t) (arg1+arg2); break;
    case INT64_T: d.value.integer = (int64_t) (arg1+arg2); break;
    case UINT32_T: d.value.integer = (uint32_t) (arg1+arg2); break;
    case UINT64_T: d.value.integer = (uint64_t) (arg1+arg2); break;
    case DOUBLE_T: d.value.floatingPoint = (double) (arg1+arg2); break;
  }
 
  return d;
}

template<>
ParseData additionHelper<char*, unsigned char>(char* n1, unsigned char n2, ParseDataType type) {

  char* arg1 = (char*) n1;
  unsigned char arg2 = (unsigned char) n2;
  uint32_t len = strlen(arg1);
  
  char* res = new char[len+2];
  res[0] = '\0';
  strncat(res, arg1, len);
  res[len] = arg2; res[len+1] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;

  return d;
}

template<>
ParseData additionHelper<char*, int32_t>(char* n1, int32_t n2, ParseDataType type) {

  char* arg1 = (char*) n1;
  int32_t arg2 = (int32_t) n2;
  const char* arg2Str = std::to_string(arg2).c_str();
  uint32_t len1 = strlen(arg1);
  uint32_t len2 = strlen(arg2Str);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1, len1);
  strncat(res, arg2Str, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<char*, int64_t>(char* n1, int64_t n2, ParseDataType type) {

  char* arg1 = (char*) n1;
  int64_t arg2 = (int64_t) n2;
  const char* arg2Str = std::to_string(arg2).c_str();
  uint32_t len1 = strlen(arg1);
  uint32_t len2 = strlen(arg2Str);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1, len1);
  strncat(res, arg2Str, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<char*, uint32_t>(char* n1, uint32_t n2, ParseDataType type) {

  char* arg1 = (char*) n1;
  uint32_t arg2 = (uint32_t) n2;
  const char* arg2Str = std::to_string(arg2).c_str();
  uint32_t len1 = strlen(arg1);
  uint32_t len2 = strlen(arg2Str);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1, len1);
  strncat(res, arg2Str, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<char*, uint64_t>(char* n1, uint64_t n2, ParseDataType type) {

  char* arg1 = (char*) n1;
  uint64_t arg2 = (uint64_t) n2;
  const char* arg2Str = std::to_string(arg2).c_str();
  uint32_t len1 = strlen(arg1);
  uint32_t len2 = strlen(arg2Str);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1, len1);
  strncat(res, arg2Str, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<char*, double>(char* n1, double n2, ParseDataType type) {

  char* arg1 = (char*) n1;
  double arg2 = (double) n2;
  const char* arg2Str = std::to_string(arg2).c_str();
  uint32_t len1 = strlen(arg1);
  uint32_t len2 = strlen(arg2Str);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1, len1);
  strncat(res, arg2Str, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<char*, char*>(char* n1, char* n2, ParseDataType type) {

  char* arg1 = (char*) n1;
  char* arg2 = (char*) n2;
  uint32_t len1 = strlen(arg1);
  uint32_t len2 = strlen(arg2);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1, len1);
  strncat(res, arg2, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<unsigned char, char*>(unsigned char n1, char* n2, ParseDataType type) {

  unsigned char arg1 = (unsigned char) n1;
  char* arg2 = (char*) n2;
  uint32_t len = strlen(arg2);
  
  char* res = new char[len+2];
  res[0] = arg1; res[1] = '\0';
  strncat(res, arg2, len);
  res[len+1] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<int32_t, char*>(int32_t n1, char* n2, ParseDataType type) {

  int32_t arg1 = (int32_t) n1;
  const char* arg1Str = std::to_string(arg1).c_str();
  char* arg2 = (char*) n2;
  uint32_t len1 = strlen(arg1Str);
  uint32_t len2 = strlen(arg2);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1Str, len1);
  strncat(res, arg2, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<int64_t, char*>(int64_t n1, char* n2, ParseDataType type) {

  int64_t arg1 = (int64_t) n1;
  const char* arg1Str = std::to_string(arg1).c_str();
  char* arg2 = (char*) n2;
  uint32_t len1 = strlen(arg1Str);
  uint32_t len2 = strlen(arg2);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1Str, len1);
  strncat(res, arg2, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<uint32_t, char*>(uint32_t n1, char* n2, ParseDataType type) {

  uint32_t arg1 = (uint32_t) n1;
  const char* arg1Str = std::to_string(arg1).c_str();
  char* arg2 = (char*) n2;
  uint32_t len1 = strlen(arg1Str);
  uint32_t len2 = strlen(arg2);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1Str, len1);
  strncat(res, arg2, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<uint64_t, char*>(uint64_t n1, char* n2, ParseDataType type) {

  uint64_t arg1 = (uint64_t) n1;
  const char* arg1Str = std::to_string(arg1).c_str();
  char* arg2 = (char*) n2;
  uint32_t len1 = strlen(arg1Str);
  uint32_t len2 = strlen(arg2);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1Str, len1);
  strncat(res, arg2, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}

template<>
ParseData additionHelper<double, char*>(double n1, char* n2, ParseDataType type) {

  double arg1 = (double) n1;
  const char* arg1Str = std::to_string(arg1).c_str();
  char* arg2 = (char*) n2;
  uint32_t len1 = strlen(arg1Str);
  uint32_t len2 = strlen(arg2);
  
  char* res = new char[len1+len2+1];
  res[0] = '\0';
  strncat(res, arg1Str, len1);
  strncat(res, arg2, len2);
  res[len1+len2] = '\0';
  
  ParseData d;
  d.type = type;
  d.value.allocated = (void*) res;
  
  return d;
}


//actual evaluating function
ParseData evaluateArithmeticExpression(ArithmeticOperatorNode* node) {
  
  //left and right arguments (calculated recursively)
  ParseData left = node->leftArg->evaluate();
  ParseData right = node->rightArg->evaluate();

  ParseData d;

  switch(node->operation) {
    
    ////////////////////////////////
    ///////     Exponent     ///////
    ////////////////////////////////
    
    case EXPONENT_OP: {

      switch(left.type) {
  
        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return exponentHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer); 
            case INT32_T: return exponentHelper((unsigned char) left.value.integer, (int32_t) right.value.integer); 
            case INT64_T: return exponentHelper((unsigned char) left.value.integer, (int64_t) right.value.integer); 
            case UINT32_T: return exponentHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer); 
            case UINT64_T: return exponentHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer); 
            case DOUBLE_T: return exponentHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint);  
          }
        }    
    
        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return exponentHelper((int32_t) left.value.integer, (unsigned char) right.value.integer); 
            case INT32_T: return exponentHelper((int32_t) left.value.integer, (int32_t) right.value.integer); 
            case INT64_T: return exponentHelper((int32_t) left.value.integer, (int64_t) right.value.integer); 
            case UINT32_T: return exponentHelper((int32_t) left.value.integer, (uint32_t) right.value.integer); 
            case UINT64_T: return exponentHelper((int32_t) left.value.integer, (uint64_t) right.value.integer); 
            case DOUBLE_T: return exponentHelper((int32_t) left.value.integer, (double) right.value.floatingPoint);               
          }
        }
    
        case INT64_T: {
    
          switch(right.type) {
            case CHAR_T: return exponentHelper((int64_t) left.value.integer, (unsigned char) right.value.integer); 
            case INT32_T: return exponentHelper((int64_t) left.value.integer, (int32_t) right.value.integer); 
            case INT64_T: return exponentHelper((int64_t) left.value.integer, (int64_t) right.value.integer); 
            case UINT32_T: return exponentHelper((int64_t) left.value.integer, (uint32_t) right.value.integer); 
            case UINT64_T: return exponentHelper((int64_t) left.value.integer, (uint64_t) right.value.integer); 
            case DOUBLE_T: return exponentHelper((int64_t) left.value.integer, (double) right.value.floatingPoint);  
          }    
        }
    
        case UINT32_T: {
    
          switch(right.type) {            
            case CHAR_T: return exponentHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer); 
            case INT32_T: return exponentHelper((uint32_t) left.value.integer, (int32_t) right.value.integer); 
            case INT64_T: return exponentHelper((uint32_t) left.value.integer, (int64_t) right.value.integer); 
            case UINT32_T: return exponentHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer); 
            case UINT64_T: return exponentHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer); 
            case DOUBLE_T: return exponentHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint);   
          }
        }
  
        case UINT64_T: {
    
          switch(right.type) {
            case CHAR_T: return exponentHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer); 
            case INT32_T: return exponentHelper((uint64_t) left.value.integer, (int32_t) right.value.integer); 
            case INT64_T: return exponentHelper((uint64_t) left.value.integer, (int64_t) right.value.integer); 
            case UINT32_T: return exponentHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer); 
            case UINT64_T: return exponentHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer); 
            case DOUBLE_T: return exponentHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint);  
          }
        }
    
        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return exponentHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer); 
            case INT32_T: return exponentHelper((double) left.value.floatingPoint, (int32_t) right.value.integer); 
            case INT64_T: return exponentHelper((double) left.value.floatingPoint, (int64_t) right.value.integer); 
            case UINT32_T: return exponentHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer); 
            case UINT64_T: return exponentHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer); 
            case DOUBLE_T: return exponentHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint);  
          }
        }
      }
    }


    ////////////////////////////////
    ///////     Division     ///////
    ////////////////////////////////

    case DIVIDE_OP: {

      ParseDataType finalType = getTypeArithmeticExpression(DIVIDE_OP, left.type, right.type);
      
      switch(left.type) {

        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return divisionHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return divisionHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return divisionHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return divisionHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return divisionHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return divisionHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, finalType);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return divisionHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return divisionHelper((int32_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return divisionHelper((int32_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return divisionHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return divisionHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return divisionHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, finalType);               
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return divisionHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return divisionHelper((int64_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return divisionHelper((int64_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return divisionHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return divisionHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return divisionHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, finalType);  
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return divisionHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return divisionHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return divisionHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return divisionHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return divisionHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return divisionHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, finalType);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return divisionHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return divisionHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return divisionHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return divisionHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return divisionHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return divisionHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, finalType);  
          }
        }

        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return divisionHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return divisionHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, finalType); 
            case INT64_T: return divisionHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return divisionHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return divisionHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return divisionHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, finalType);  
          }
        }
      }
    }
    

    
    ////////////////////////////////
    ////     Multiplication     ////
    ////////////////////////////////

    case MULTIPLY_OP: {
  
      //string operation case
      if(left.type == STRING_T || right.type == STRING_T) {
  
        char* str = (char*) ((left.type == STRING_T) ? left.value.allocated :
                             right.value.allocated);

        uint64_t num = (left.type == STRING_T) ? right.value.integer : left.value.integer;
        ParseDataType intType = (left.type == STRING_T) ? right.type : left.type;

        uint64_t val = 0;
        bool rev = false;
        
        switch(intType) {
          
          case INT8_T: {
            int8_t v = (int8_t) num;
            if(v < 0) {
              rev = true;
              val = -v;
            } else {
              rev = false;
              val = v;
            }
            break;
          }

          case INT16_T: {
            int16_t v = (int16_t) num;
            if(v < 0) {
              rev = true;
              val = -v;
            } else {
              rev = false;
              val = v;
            }
            break;
          }

          case INT32_T: {
            int32_t v = (int32_t) num;
            if(v < 0) {
              rev = true;
              val = -v;
            } else {
              rev = false;
              val = v;
            }
            break;
          }

          case INT64_T: {
            int64_t v = (int64_t) num;
            if(v < 0) {
              rev = true;
              val = -v;
            } else {
              rev = false;
              val = v;
            }
            break;
          }

          case UINT8_T: {
            uint8_t v = (uint8_t) num;
            rev = false;
            val = v;
            break;
          }

          case CHAR_T: {
            char v = (char) num;
            if(v < 0) {
              rev = true;
              val = -v;
            } else {
              rev = false;
              val = v;
            }
            break; 
          }

          case UINT16_T: {
            uint16_t v = (uint16_t) num;
            rev = false;
            val = v;
            break;
          }

          case UINT32_T: {
            uint32_t v = (uint32_t) num;
            rev = false;
            val = v;
            break;
          }

          case UINT64_T: {
            uint64_t v = (uint64_t) num;
            rev = false;
            val = v;
            break;
          }
        }

        uint64_t len = strlen(str);
        uint64_t finalLen = len*val;
        char* c = new char[finalLen + 1];
        c[0] = '\0';

        for(uint64_t i = 0; i < val; i++) {
          strncat(c, str, len);
        }
        c[finalLen] = '\0';

        d.type = STRING_T;

        //if val is negative, reverse the string
        if(rev) {
          
          char* c2 = new char[finalLen + 1];
          for(uint64_t i = 0; i < finalLen; i++) {
            c2[i] = c[finalLen-1-i];
          } 
          c2[finalLen] = '\0';
          d.value.allocated = (void*) c2;

        } else {
          d.value.allocated = (void*) c;
        }

      } else {

        ParseDataType finalType = getTypeArithmeticExpression(MULTIPLY_OP, left.type, right.type);
    
        switch(left.type) {

          case CHAR_T: {
            
            switch(right.type) {
              case CHAR_T: return multiplicationHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, finalType); 
              case INT32_T: return multiplicationHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, finalType); 
              case INT64_T: return multiplicationHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, finalType); 
              case UINT32_T: return multiplicationHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, finalType); 
              case UINT64_T: return multiplicationHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, finalType); 
              case DOUBLE_T: return multiplicationHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, finalType);  
            }
          }    

          case INT32_T: {
            
            switch(right.type) {
              case CHAR_T: return multiplicationHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
              case INT32_T: return multiplicationHelper((int32_t) left.value.integer, (int32_t) right.value.integer, finalType); 
              case INT64_T: return multiplicationHelper((int32_t) left.value.integer, (int64_t) right.value.integer, finalType); 
              case UINT32_T: return multiplicationHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
              case UINT64_T: return multiplicationHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
              case DOUBLE_T: return multiplicationHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, finalType);               
            }
          }

          case INT64_T: {

            switch(right.type) {
              case CHAR_T: return multiplicationHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
              case INT32_T: return multiplicationHelper((int64_t) left.value.integer, (int32_t) right.value.integer, finalType); 
              case INT64_T: return multiplicationHelper((int64_t) left.value.integer, (int64_t) right.value.integer, finalType); 
              case UINT32_T: return multiplicationHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
              case UINT64_T: return multiplicationHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
              case DOUBLE_T: return multiplicationHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, finalType);  
            }    
          }

          case UINT32_T: {

            switch(right.type) {            
              case CHAR_T: return multiplicationHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
              case INT32_T: return multiplicationHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, finalType); 
              case INT64_T: return multiplicationHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, finalType); 
              case UINT32_T: return multiplicationHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
              case UINT64_T: return multiplicationHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
              case DOUBLE_T: return multiplicationHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, finalType);   
            }
          }

          case UINT64_T: {

            switch(right.type) {
              case CHAR_T: return multiplicationHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
              case INT32_T: return multiplicationHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, finalType); 
              case INT64_T: return multiplicationHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, finalType); 
              case UINT32_T: return multiplicationHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
              case UINT64_T: return multiplicationHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
              case DOUBLE_T: return multiplicationHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, finalType);  
            }
          }

          case DOUBLE_T: {
            
            switch(right.type) {
              case CHAR_T: return multiplicationHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, finalType); 
              case INT32_T: return multiplicationHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, finalType); 
              case INT64_T: return multiplicationHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, finalType); 
              case UINT32_T: return multiplicationHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, finalType); 
              case UINT64_T: return multiplicationHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, finalType); 
              case DOUBLE_T: return multiplicationHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, finalType);  
            }
          }
        }
        
      }
	  
      break;  //end operation case
    }

    ////////////////////////////////
    ///////      Modulus     ///////
    ////////////////////////////////
    
    case MOD_OP: {

      ParseDataType finalType = getTypeArithmeticExpression(MOD_OP, left.type, right.type);
      
      switch(left.type) {

        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return modHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return modHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return modHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return modHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return modHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return modHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, finalType);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return modHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return modHelper((int32_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return modHelper((int32_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return modHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return modHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return modHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, finalType);               
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return modHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return modHelper((int64_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return modHelper((int64_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return modHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return modHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return modHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, finalType);  
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return modHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return modHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return modHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return modHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return modHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return modHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, finalType);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return modHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return modHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return modHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return modHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return modHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return modHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, finalType);  
          }
        }

        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return modHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return modHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, finalType); 
            case INT64_T: return modHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return modHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return modHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return modHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, finalType);  
          }
        }
      }
    }
    
 
    ////////////////////////////////
    /////         Add          /////
    ////////////////////////////////
    
    case ADD_OP: {

      ParseDataType finalType = getTypeArithmeticExpression(ADD_OP, left.type, right.type);
      
      switch(left.type) {

        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return additionHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return additionHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return additionHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return additionHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return additionHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return additionHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, finalType); 
            case STRING_T: return additionHelper((unsigned char) left.value.integer, (char*) right.value.allocated, finalType);         
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return additionHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return additionHelper((int32_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return additionHelper((int32_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return additionHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return additionHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return additionHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, finalType); 
            case STRING_T: return additionHelper((int32_t) left.value.integer, (char*) right.value.allocated, finalType); 
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return additionHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return additionHelper((int64_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return additionHelper((int64_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return additionHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return additionHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return additionHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, finalType); 
            case STRING_T: return additionHelper((int64_t) left.value.integer, (char*) right.value.allocated, finalType); 
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return additionHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return additionHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return additionHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return additionHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return additionHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return additionHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, finalType);
            case STRING_T: return additionHelper((uint32_t) left.value.integer, (char*) right.value.allocated, finalType);
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return additionHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return additionHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return additionHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return additionHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return additionHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return additionHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, finalType);
            case STRING_T: return additionHelper((uint64_t) left.value.integer, (char*) right.value.allocated, finalType);
          }
        }

        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return additionHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return additionHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, finalType); 
            case INT64_T: return additionHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return additionHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return additionHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return additionHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, finalType); 
            case STRING_T: return additionHelper((double) left.value.floatingPoint, (char*) right.value.allocated, finalType); 
          }
        }
        
        case STRING_T: {
          
          switch(right.type) {
            case CHAR_T: return additionHelper((char*) left.value.allocated, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return additionHelper((char*) left.value.allocated, (int32_t) right.value.integer, finalType); 
            case INT64_T: return additionHelper((char*) left.value.allocated, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return additionHelper((char*) left.value.allocated, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return additionHelper((char*) left.value.allocated, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return additionHelper((char*) left.value.allocated, (double) right.value.floatingPoint, finalType); 
            case STRING_T: return additionHelper((char*) left.value.allocated, (char*) right.value.allocated, finalType); 
          }
        }
      }
    }

    
    ////////////////////////////////
    ///////      Subtract     //////
    ////////////////////////////////
    
    case SUBTRACT_OP: {

      ParseDataType finalType = getTypeArithmeticExpression(SUBTRACT_OP, left.type, right.type);
      
      switch(left.type) {

        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return subtractionHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return subtractionHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return subtractionHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return subtractionHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return subtractionHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return subtractionHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, finalType);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return subtractionHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return subtractionHelper((int32_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return subtractionHelper((int32_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return subtractionHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return subtractionHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return subtractionHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, finalType);               
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return subtractionHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return subtractionHelper((int64_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return subtractionHelper((int64_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return subtractionHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return subtractionHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return subtractionHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, finalType);  
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return subtractionHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return subtractionHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return subtractionHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return subtractionHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return subtractionHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return subtractionHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, finalType);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return subtractionHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return subtractionHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, finalType); 
            case INT64_T: return subtractionHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return subtractionHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return subtractionHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return subtractionHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, finalType);  
          }
        }

        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return subtractionHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, finalType); 
            case INT32_T: return subtractionHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, finalType); 
            case INT64_T: return subtractionHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, finalType); 
            case UINT32_T: return subtractionHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, finalType); 
            case UINT64_T: return subtractionHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, finalType); 
            case DOUBLE_T: return subtractionHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, finalType);  
          }
        }
      }
    }
    
  }

  return d;
}