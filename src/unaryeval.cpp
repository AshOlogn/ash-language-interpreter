#include <cstdint>
#include <cmath>
#include <cstring>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"


ParseData evaluateUnaryExpression(UnaryOperatorNode* node) {
  
  ParseData arg = node->leftArg->evaluate();
  ParseData d;
  d.type = arg.type;
  
  switch(node->operation) {
    
    case POSITIVE_OP: {
      
      switch(arg.type) {
        
        case CHAR_T: {
          unsigned char val = (unsigned char) arg.value.integer;
          d.value.integer = val;
          return d;
        }
        
        case INT32_T: {
          int32_t val = (int32_t) arg.value.integer;
          d.value.integer = val;
          return d;
        }
        
        case INT64_T: {
          int64_t val = (int64_t) arg.value.integer;
          d.value.integer = val;
          return d;
        }
        
        case UINT32_T:  {
          uint32_t val = (uint32_t) arg.value.integer;
          d.value.integer = val;
          return d;
        }
        
        case UINT64_T: {
          uint64_t val = (uint64_t) arg.value.integer;
          d.value.integer = val;
          return d;
        }
        
        case DOUBLE_T: {
          double val = (double) arg.value.floatingPoint;
          d.value.floatingPoint = (double) val;
          return d;
        }
        
      }
    }
    
    case NEGATIVE_OP: {
      
      switch(arg.type) {
        
        case CHAR_T: {
          unsigned char val = (unsigned char) arg.value.integer;
          d.value.integer = -val;
          return d;
        }
        
        case INT32_T: {
          int32_t val = (int32_t) arg.value.integer;
          d.value.integer = -val;
          return d;
        }
        
        case INT64_T: {
          int64_t val = (int64_t) arg.value.integer;
          d.value.integer = -val;
          return d;
        }
        
        case UINT32_T:  {
          uint32_t val = (uint32_t) arg.value.integer;
          d.value.integer = -val;
          return d;
        }
        
        case UINT64_T: {
          uint64_t val = (uint64_t) arg.value.integer;
          d.value.integer = -val;
          return d;
        }
        
        case DOUBLE_T: {
          double val = (double) arg.value.floatingPoint;
          d.value.floatingPoint = -val;
          return d;
        }
        
      }
    }
    
    case BIT_NOT_OP: {
      
      switch(arg.type) {
        
        case CHAR_T: {
          unsigned char val = (unsigned char) arg.value.integer;
          d.value.integer = ~val;
          return d;
        }
        
        case INT32_T: {
          int32_t val = (int32_t) arg.value.integer;
          d.value.integer = ~val;
          return d;
        }
        
        case INT64_T: {
          int64_t val = (int64_t) arg.value.integer;
          d.value.integer = ~val;
          return d;
        }
        
        case UINT32_T:  {
          uint32_t val = (uint32_t) arg.value.integer;
          d.value.integer = val;
          return d;
        }
        
        case UINT64_T: {
          uint64_t val = (uint64_t) arg.value.integer;
          d.value.integer = ~val;
          return d;
        }
      }
    }
    
    case NOT_OP: {
      bool val = (bool) arg.value.integer;
      d.value.integer = !val;
      return d;
    }
    
  }
  
  return d;
}