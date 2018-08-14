#include <cstdint>
#include <cmath>
#include <cstring>
#include <string>
#include <iostream>
#include "utils.h"
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "casteval.h"

using namespace std;

//helper function to deal with casting cases
ParseData castHelper(ParseData orig, ParseDataType finalType) {
  
  ParseDataType origType = orig.type;
  ParseData d;
  d.type = finalType;
  
  switch(origType) {
        
		case ARRAY_T: {
			if(finalType == ARRAY_T)
				d.value.allocated = orig.value.allocated;
			break;
		}
    
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
          d.value.allocated = (void*) copyString((orig.value.integer) ? "true" : "false");
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
          d.value.allocated = (void*) copyString(std::to_string((int32_t) orig.value.integer).c_str());
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
          d.value.allocated = (void*) copyString(std::to_string((int64_t) orig.value.integer).c_str());
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
          d.value.allocated = (void*) copyString(std::to_string((uint32_t) orig.value.integer).c_str());
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
          d.value.allocated = (void*) copyString(std::to_string((uint64_t) orig.value.integer).c_str());
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
          d.value.allocated = (void*) copyString(std::to_string((double) orig.value.floatingPoint).c_str());
          break;
        }
      }
      break;
    }
    
    case STRING_T: {
      if(finalType == STRING_T) {
        d.value.allocated = (void*) copyString((char*) orig.value.allocated);
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

