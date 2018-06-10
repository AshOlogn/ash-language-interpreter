#include <iostream>
#include <cstdint>
#include <cmath>
#include <cstring>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"

//recursively evaluate AST, start at root
ParseData evaluate(AbstractExpressionNode* node) {
  return node->evaluate();
}


//evaluate expression appropriately depending on type of AbstractExpressionNode
ParseData evaluateUnaryExpression(AbstractExpressionNode* node) {
  
  ParseData d;
  return d;
}

ParseData evaluateArithmeticExpression(ArithmeticOperatorNode* node) {
  
  //left and right arguments (calculated recursively)
  ParseData left = node->leftArg->evaluate();
  ParseData right = node->rightArg->evaluate();
  

  ParseData d;

  switch(node->operation) {
    
    case EXPONENT_OP: {

      double arg1 = isIntParseDataType(left.type) ? (double) left.value.integer :
                    left.value.floatingPoint;

      double arg2 = isIntParseDataType(right.type) ? (double) right.value.integer :
                    right.value.floatingPoint;

      d.type = DOUBLE_T;  
      d.value.floatingPoint = pow(arg1, arg2);
      break;
    }

    case DIVIDE_OP: {
 
      ParseDataType finalType = getLargerNumberParseDataType(left.type, right.type);
 
      if(finalType == DOUBLE_T) {
  
        double arg1 = isIntParseDataType(left.type) ? (double) left.value.integer :
                      left.value.floatingPoint;

        double arg2 = isIntParseDataType(right.type) ? (double) right.value.integer :
                      right.value.floatingPoint;

        d.type = DOUBLE_T;  
        d.value.floatingPoint = arg1/arg2;

      } else {
        
        switch(finalType) {
  
          case INT8_T: {
            int8_t val = (int8_t) (left.value.integer/right.value.integer);
            d.value.integer = val;
            break;
          }

          case INT16_T: {
            int16_t val = (int16_t) (left.value.integer/right.value.integer);
            d.value.integer = val;  
            break;   
          }

          case INT32_T: {
            int32_t val = (int32_t) (left.value.integer/right.value.integer);
            d.value.integer = val;
            break;     
          }

          case INT64_T: {
            int64_t val = (int64_t) (left.value.integer/right.value.integer);
            d.value.integer = val; 
            break;
          }

          case UINT8_T: {
            uint8_t val = (uint8_t) (left.value.integer/right.value.integer);
            d.value.integer = val;
            break;
          }

          case UINT16_T: {
            uint16_t val = (uint16_t) (left.value.integer/right.value.integer);
            d.value.integer = val;
            break;
          }

          case UINT32_T: {
            uint32_t val = (uint32_t) (left.value.integer/right.value.integer);
            d.value.integer = val;
            break;
          }

          case UINT64_T: {
            uint64_t val = (uint64_t) (left.value.integer/right.value.integer);
            d.value.integer = val;
            break;
          }

          case CHAR_T: {
            char val = (char) (left.value.integer/right.value.integer);
            d.value.integer = val;
            break;
          }
        }

        d.type = finalType;
      }

      break;
    }

    case MULTIPLY_OP: {
  
      //string case
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

      } else if(left.type == DOUBLE_T || right.type == DOUBLE_T) {
 
        double arg1 = isIntParseDataType(left.type) ? (double) left.value.integer :
                      left.value.floatingPoint;

        double arg2 = isIntParseDataType(right.type) ? (double) right.value.integer :
                      right.value.floatingPoint;

        d.type = DOUBLE_T;  
        d.value.floatingPoint = arg1*arg2;

      } else {

        ParseDataType finalType = getLargerNumberParseDataType(left.type, right.type);
        switch(finalType) {
  
          case INT8_T: {
            int8_t val = (int8_t) (left.value.integer*right.value.integer);
            d.value.integer = val;
            break;
          }

          case INT16_T: {
            int16_t val = (int16_t) (left.value.integer*right.value.integer);
            d.value.integer = val;  
            break;   
          }

          case INT32_T: {
            int32_t val = (int32_t) (left.value.integer*right.value.integer);
            d.value.integer = val;
            break;     
          }

          case INT64_T: {
            int64_t val = (int64_t) (left.value.integer*right.value.integer);
            d.value.integer = val; 
            break;
          }

          case UINT8_T: {
            uint8_t val = (uint8_t) (left.value.integer*right.value.integer);
            d.value.integer = val;
            break;
          }

          case UINT16_T: {
            uint16_t val = (uint16_t) (left.value.integer*right.value.integer);
            d.value.integer = val;
            break;
          }

          case UINT32_T: {
            uint32_t val = (uint32_t) (left.value.integer*right.value.integer);
            d.value.integer = val;
            break;
          }

          case UINT64_T: {
            uint64_t val = (uint64_t) (left.value.integer*right.value.integer);
            d.value.integer = val;
            break;
          }

          case CHAR_T: {
            char val = (char) (left.value.integer*right.value.integer);
            d.value.integer = val;
            break;
          }
        }

        d.type = finalType;
      }

      break;
    }

    case MOD_OP: break;
    case ADD_OP: break;
    case SUBTRACT_OP: break;

  }

  return d;
}

ParseData evaluateBitLogicalExpression(AbstractExpressionNode* node) {
  
  ParseData d;
  return d;
}

ParseData evaluateComparisonExpression(AbstractExpressionNode* node) {
  
  ParseData d;
  return d;
}

ParseData evaluateAssignmentExpression(AbstractExpressionNode* node) {
  
  ParseData d;
  return d;
}

//just return the value represented by node's ParseData
ParseData evaluateLiteralExpression(AbstractExpressionNode* node) {
  return ((LiteralNode*) node)->data;
}

//just evaluate subtree representing parenthesized expression
ParseData evaluateGroupedExpression(AbstractExpressionNode* node) {
  return ((GroupedExpressionNode*) node)->closedExpression->evaluate();
}

