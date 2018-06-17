#include <cstdint>
#include <cmath>
#include <cstring>
#include <string>
#include <iostream>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "casteval.h"

using namespace std;

//helper function
ParseData castHelper(ParseData orig, ParseDataType finalType) {
  
  ParseDataType origType = orig.type;
  ParseData d;
  d.type = finalType;
  
  switch(origType) {
        
    case BOOL_T: {
      
      switch(finalType) {
        case BOOL_T: d.value.integer = orig.value.integer; break;
        case CHAR_T: d.value.integer = (unsigned char) orig.value.integer; break;
        case INT32_T: d.value.integer = (int32_t) orig.value.integer; break;
        case INT64_T: d.value.integer = (int64_t) orig.value.integer; break;
        case UINT32_T: d.value.integer = (uint32_t) orig.value.integer; break;
        case UINT64_T: d.value.integer = (uint64_t) orig.value.integer; break;
        case DOUBLE_T: d.value.floatingPoint = (double) orig.value.integer; break;
        case STRING_T: {
          const char* temp = (orig.value.integer) ? "true" : "false";
          uint32_t len = strlen(temp);
          char* res = new char[len+1];
          strcpy(res, temp);
          res[len] = '\0';
          d.value.allocated = (void*) res;
          break;
        }
      } 
    }
    
    case CHAR_T: {
      switch(finalType) {
        case BOOL_T: d.value.integer = (orig.value.integer) ? 1 : 0; break;
        case CHAR_T: d.value.integer = (unsigned char) ((unsigned char) orig.value.integer); break;
        case INT32_T: d.value.integer = (int32_t) ((unsigned char) orig.value.integer); break;
        case INT64_T: d.value.integer = (int64_t) ((unsigned char) orig.value.integer); break;
        case UINT32_T: d.value.integer = (uint32_t) ((unsigned char) orig.value.integer); break;
        case UINT64_T: d.value.integer = (uint64_t) ((unsigned char) orig.value.integer); break;
        case DOUBLE_T: d.value.floatingPoint = (double) ((unsigned char) orig.value.integer); break;
        case STRING_T: {
          char* res = new char[2];
          res[0] = (unsigned char) orig.value.integer;
          res[1] = '\0';
          d.value.allocated = (void*) res;
          break;
        }
      }
      break;
    }
    
    case INT32_T: {
      switch(finalType) {
        case BOOL_T: d.value.integer = (orig.value.integer) ? 1 : 0; break;
        case CHAR_T: d.value.integer = (unsigned char) ((int32_t) orig.value.integer); break;
        case INT32_T: d.value.integer = (int32_t) orig.value.integer; break;
        case INT64_T: d.value.integer = (int64_t) ((int32_t) orig.value.integer); break;
        case UINT32_T: d.value.integer = (uint32_t) ((int32_t) orig.value.integer); break;
        case UINT64_T: d.value.integer = (uint64_t) ((int32_t) orig.value.integer); break;
        case DOUBLE_T: d.value.floatingPoint = (double) ((int32_t) orig.value.integer); break;
        case STRING_T: {
          const char* temp = std::to_string((int32_t) orig.value.integer).c_str();
          uint32_t len = strlen(temp);
          char* res = new char[len+1];
          strcpy(res, temp);
          res[len] = '\0';
          d.value.allocated = (void*) res;
          break;
        }
      }
      break;
    }
    
    case INT64_T: {
      switch(finalType) {
        case BOOL_T: d.value.integer = (orig.value.integer) ? 1 : 0; break;
        case CHAR_T: d.value.integer = (unsigned char) ((int64_t) orig.value.integer); break;
        case INT32_T: d.value.integer = (int32_t) ((int64_t) orig.value.integer); break;
        case INT64_T: d.value.integer = (int64_t) ((int64_t) orig.value.integer); break;
        case UINT32_T: d.value.integer = (uint32_t) ((int64_t) orig.value.integer); break;
        case UINT64_T: d.value.integer = (uint64_t) ((int64_t) orig.value.integer); break;
        case DOUBLE_T: d.value.floatingPoint = (double) ((int64_t) orig.value.integer); break;
        case STRING_T: {
          const char* temp = std::to_string((int64_t) orig.value.integer).c_str();
          uint32_t len = strlen(temp);
          char* res = new char[len+1];
          strcpy(res, temp);
          res[len] = '\0';
          d.value.allocated = (void*) res;
          break;
        }
      }
      break;
    }
    
    case UINT32_T: {
      switch(finalType) {
        case BOOL_T: d.value.integer = (orig.value.integer) ? 1 : 0; break;
        case CHAR_T: d.value.integer = (unsigned char) ((uint32_t) orig.value.integer); break;
        case INT32_T: d.value.integer = (int32_t) orig.value.integer; break;
        case INT64_T: d.value.integer = (int64_t) ((uint32_t) orig.value.integer); break;
        case UINT32_T: d.value.integer = (uint32_t) ((uint32_t) orig.value.integer); break;
        case UINT64_T: d.value.integer = (uint64_t) ((uint32_t) orig.value.integer); break;
        case DOUBLE_T: d.value.floatingPoint = (double) ((uint32_t) orig.value.integer); break;
        case STRING_T: {
          const char* temp = std::to_string((uint32_t) orig.value.integer).c_str();
          uint32_t len = strlen(temp);
          char* res = new char[len+1];
          strcpy(res, temp);
          res[len] = '\0';
          d.value.allocated = (void*) res;
          break;
        }
      }
      break;
    }
    
    case UINT64_T: {
      switch(finalType) {
        case BOOL_T: d.value.integer = (orig.value.integer) ? 1 : 0; break;
        case CHAR_T: d.value.integer = (unsigned char) ((uint64_t) orig.value.integer); break;
        case INT32_T: d.value.integer = (int32_t) ((uint64_t) orig.value.integer); break;
        case INT64_T: d.value.integer = (int64_t) ((uint64_t) orig.value.integer); break;
        case UINT32_T: d.value.integer = (uint32_t) ((uint64_t) orig.value.integer); break;
        case UINT64_T: d.value.integer = (uint64_t) ((uint64_t) orig.value.integer); break;
        case DOUBLE_T: d.value.floatingPoint = (double) ((uint64_t) orig.value.integer); break;
        case STRING_T: {
          const char* temp = std::to_string((uint64_t) orig.value.integer).c_str();
          uint32_t len = strlen(temp);
          char* res = new char[len+1];
          strcpy(res, temp);
          res[len] = '\0';
          d.value.allocated = (void*) res;
          break;
        }
      }
      break;
    }
    
    case DOUBLE_T: {
      
      switch(finalType) {
        case BOOL_T: d.value.integer = (std::abs((double) orig.value.floatingPoint) > 1.0e-16) ? 1 : 0; break;
        case CHAR_T: d.value.integer = (unsigned char) ((double) orig.value.floatingPoint); break;  
        case INT32_T: d.value.integer = (int32_t) ((double) orig.value.floatingPoint); break; 
        case INT64_T: d.value.integer = (int64_t) ((double) orig.value.floatingPoint); break; 
        case UINT32_T: d.value.integer = (uint32_t) ((double) orig.value.floatingPoint); break; 
        case UINT64_T: d.value.integer = (uint64_t) ((double) orig.value.floatingPoint); break; 
        case DOUBLE_T: d.value.floatingPoint = orig.value.floatingPoint; break;
        case STRING_T: {
          const char* temp = std::to_string((double) orig.value.floatingPoint).c_str();
          uint32_t len = strlen(temp);
          char* res = new char[len+1];
          strcpy(res, temp);
          res[len] = '\0';
          d.value.allocated = (void*) res;
          break;
        }
      }
      break;
    }
    
    case STRING_T: {
      if(finalType == STRING_T) {
        const char* temp = (char*) orig.value.allocated;
        uint32_t len = strlen(temp);
        char* res = new char[len+1];
        strcpy(res, temp);
        res[len] = '\0';
        d.value.allocated = (void*) res;
      }
      
      break;
    }
    
  }
  
  return d;
}


ParseData evaluateCastExpression(CastNode* node) {
  ParseData arg = node->expression->evaluate();
  ParseData castData =  castHelper(arg, node->finalType);
	return castData;
}

