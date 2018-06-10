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


    ////////////////////////////////
    ///////     Division     ///////
    ////////////////////////////////

    case DIVIDE_OP: { 

      //branch on numerator's type
      switch(left.type) {

        case CHAR_T: {

          //branch on denominator's type
          switch(right.type) {

            case CHAR_T: {
  
              unsigned char num = (unsigned char) left.value.integer; 
              unsigned char den = (unsigned char) right.value.integer;
              d.type = CHAR_T;
              d.value.integer = (unsigned char) (num/den);        
              break;
            }

            case INT32_T: {
  
              unsigned char num = (unsigned char) left.value.integer; 
              int32_t den = (int32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (num/den);        
              break;
            }

            case INT64_T: {
  
              unsigned char num = (unsigned char) left.value.integer; 
              int64_t den = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case UINT32_T: {
  
              unsigned char num = (unsigned char) left.value.integer; 
              uint32_t den = (uint32_t) right.value.integer;
              d.type = UINT32_T;
              d.value.integer = (uint32_t) (num/den);        
              break;
            }

            case UINT64_T: {
  
              unsigned char num = (unsigned char) left.value.integer; 
              uint64_t den = (uint64_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (num/den);        
              break;
            }

            case DOUBLE_T: {
  
              unsigned char num = (unsigned char) left.value.integer; 
              double den = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case INT32_T: {

          //branch on denominator's type
          switch(right.type) {

            case CHAR_T: {
  
              int32_t num = (int32_t) left.value.integer; 
              unsigned char den = (unsigned char) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (num/den);        
              break;
            }

            case INT32_T: {
  
              int32_t num = (int32_t) left.value.integer; 
              int32_t den = (int32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (num/den);        
              break;
            }

            case INT64_T: {
  
              int32_t num = (int32_t) left.value.integer; 
              int64_t den = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case UINT32_T: {
  
              int32_t num = (int32_t) left.value.integer; 
              uint32_t den = (uint32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (num/den);        
              break;
            }

            case UINT64_T: {
  
              int32_t num = (int32_t) left.value.integer; 
              uint64_t den = (uint64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case DOUBLE_T: {
  
              int32_t num = (int32_t) left.value.integer; 
              double den = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case INT64_T: {

          //branch on denominator's type
          switch(right.type) {

            case CHAR_T: {
  
              int64_t num = (int64_t) left.value.integer; 
              unsigned char den = (unsigned char) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case INT32_T: {
  
              int64_t num = (int64_t) left.value.integer; 
              int32_t den = (int32_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case INT64_T: {
  
              int64_t num = (int64_t) left.value.integer; 
              int64_t den = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case UINT32_T: {
  
              int64_t num = (int64_t) left.value.integer; 
              uint32_t den = (uint32_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case UINT64_T: {
  
              int64_t num = (int64_t) left.value.integer; 
              uint64_t den = (uint64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case DOUBLE_T: {
  
              int64_t num = (int64_t) left.value.integer; 
              double den = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case UINT32_T: {

          //branch on denominator's type
          switch(right.type) {

            case CHAR_T: {
  
              uint32_t num = (uint32_t) left.value.integer; 
              unsigned char den = (unsigned char) right.value.integer;
              d.type = UINT32_T;
              d.value.integer = (uint32_t) (num/den);        
              break;
            }

            case INT32_T: {
  
              uint32_t num = (uint32_t) left.value.integer; 
              int32_t den = (int32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (num/den);        
              break;
            }

            case INT64_T: {
  
              uint32_t num = (uint32_t) left.value.integer; 
              int64_t den = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case UINT32_T: {
  
              uint32_t num = (uint32_t) left.value.integer; 
              uint32_t den = (uint32_t) right.value.integer;
              d.type = UINT32_T;
              d.value.integer = (uint32_t) (num/den);        
              break;
            }

            case UINT64_T: {
  
              uint32_t num = (uint32_t) left.value.integer; 
              uint64_t den = (uint64_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (num/den);        
              break;
            }

            case DOUBLE_T: {
  
              uint32_t num = (uint32_t) left.value.integer; 
              double den = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case UINT64_T: {

          //branch on denominator's type
          switch(right.type) {

            case CHAR_T: {
  
              uint64_t num = (uint64_t) left.value.integer; 
              unsigned char den = (unsigned char) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (num/den);        
              break;
            }

            case INT32_T: {
  
              uint64_t num = (uint64_t) left.value.integer; 
              int32_t den = (int32_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case INT64_T: {
  
              uint64_t num = (uint64_t) left.value.integer; 
              int64_t den = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (num/den);        
              break;
            }

            case UINT32_T: {
  
              uint64_t num = (uint64_t) left.value.integer; 
              uint32_t den = (uint32_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (num/den);        
              break;
            }

            case UINT64_T: {
  
              uint64_t num = (uint64_t) left.value.integer; 
              uint64_t den = (uint64_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (num/den);        
              break;
            }

            case DOUBLE_T: {
  
              uint64_t num = (uint64_t) left.value.integer; 
              double den = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case

        case DOUBLE_T: {

          //branch on denominator's type
          switch(right.type) {

            case CHAR_T: {
  
              double num = (double) left.value.floatingPoint;
              unsigned char den = (unsigned char) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

            case INT32_T: {
  
              double num = (double) left.value.floatingPoint;
              int32_t den = (int32_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

            case INT64_T: {
  
              double num = (double) left.value.floatingPoint;
              int64_t den = (int64_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

            case UINT32_T: {
  
              double num = (double) left.value.floatingPoint;
              uint32_t den = (uint32_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

            case UINT64_T: {
  
              double num = (double) left.value.floatingPoint;
              uint64_t den = (uint64_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

            case DOUBLE_T: {
  
              double num = (double) left.value.floatingPoint;
              double den = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (num/den);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case

        break;
      }

      break; //end operation case

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

        //branch on first factor's type
        switch(left.type) {

          case CHAR_T: {

            //branch on second factor's type
            switch(right.type) {

              case CHAR_T: {
    
                unsigned char arg1 = (unsigned char) left.value.integer; 
                unsigned char arg2 = (unsigned char) right.value.integer;
                d.type = CHAR_T;
                d.value.integer = (unsigned char) (arg1*arg2);        
                break;
              }

              case INT32_T: {
    
                unsigned char arg1 = (unsigned char) left.value.integer; 
                int32_t arg2 = (int32_t) right.value.integer;
                d.type = INT32_T;
                d.value.integer = (int32_t) (arg1*arg2);        
                break;
              }

              case INT64_T: {
    
                unsigned char arg1 = (unsigned char) left.value.integer; 
                int64_t arg2 = (int64_t) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case UINT32_T: {
    
                unsigned char arg1 = (unsigned char) left.value.integer; 
                uint32_t arg2 = (uint32_t) right.value.integer;
                d.type = UINT32_T;
                d.value.integer = (uint32_t) (arg1*arg2);        
                break;
              }

              case UINT64_T: {
    
                unsigned char arg1 = (unsigned char) left.value.integer; 
                uint64_t arg2 = (uint64_t) right.value.integer;
                d.type = UINT64_T;
                d.value.integer = (uint64_t) (arg1*arg2);        
                break;
              }

              case DOUBLE_T: {
    
                unsigned char arg1 = (unsigned char) left.value.integer; 
                double arg2 = (double) right.value.floatingPoint;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

            } //end inner switch

            break;

          } //end outer case


          case INT32_T: {

            //branch on second factor's type
            switch(right.type) {

              case CHAR_T: {
    
                int32_t arg1 = (int32_t) left.value.integer; 
                unsigned char arg2 = (unsigned char) right.value.integer;
                d.type = INT32_T;
                d.value.integer = (int32_t) (arg1*arg2);        
                break;
              }

              case INT32_T: {
    
                int32_t arg1 = (int32_t) left.value.integer; 
                int32_t arg2 = (int32_t) right.value.integer;
                d.type = INT32_T;
                d.value.integer = (int32_t) (arg1*arg2);        
                break;
              }

              case INT64_T: {
    
                int32_t arg1 = (int32_t) left.value.integer; 
                int64_t arg2 = (int64_t) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case UINT32_T: {
    
                int32_t arg1 = (int32_t) left.value.integer; 
                uint32_t arg2 = (uint32_t) right.value.integer;
                d.type = INT32_T;
                d.value.integer = (int32_t) (arg1*arg2);        
                break;
              }

              case UINT64_T: {
    
                int32_t arg1 = (int32_t) left.value.integer; 
                uint64_t arg2 = (uint64_t) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case DOUBLE_T: {
    
                int32_t arg1 = (int32_t) left.value.integer; 
                double arg2 = (double) right.value.floatingPoint;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

            } //end inner switch

            break;

          } //end outer case


          case INT64_T: {

            //branch on second factor's type
            switch(right.type) {

              case CHAR_T: {
    
                int64_t arg1 = (int64_t) left.value.integer; 
                unsigned char arg2 = (unsigned char) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case INT32_T: {
    
                int64_t arg1 = (int64_t) left.value.integer; 
                int32_t arg2 = (int32_t) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case INT64_T: {
    
                int64_t arg1 = (int64_t) left.value.integer; 
                int64_t arg2 = (int64_t) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case UINT32_T: {
    
                int64_t arg1 = (int64_t) left.value.integer; 
                uint32_t arg2 = (uint32_t) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case UINT64_T: {
    
                int64_t arg1 = (int64_t) left.value.integer; 
                uint64_t arg2 = (uint64_t) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case DOUBLE_T: {
    
                int64_t arg1 = (int64_t) left.value.integer; 
                double arg2 = (double) right.value.floatingPoint;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

            } //end inner switch

            break;

          } //end outer case


          case UINT32_T: {

            //branch on second factor's type
            switch(right.type) {

              case CHAR_T: {
    
                uint32_t arg1 = (uint32_t) left.value.integer; 
                unsigned char arg2 = (unsigned char) right.value.integer;
                d.type = UINT32_T;
                d.value.integer = (uint32_t) (arg1*arg2);        
                break;
              }

              case INT32_T: {
    
                uint32_t arg1 = (uint32_t) left.value.integer; 
                int32_t arg2 = (int32_t) right.value.integer;
                d.type = INT32_T;
                d.value.integer = (int32_t) (arg1*arg2);        
                break;
              }

              case INT64_T: {
    
                uint32_t arg1 = (uint32_t) left.value.integer; 
                int64_t arg2 = (int64_t) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case UINT32_T: {
    
                uint32_t arg1 = (uint32_t) left.value.integer; 
                uint32_t arg2 = (uint32_t) right.value.integer;
                d.type = UINT32_T;
                d.value.integer = (uint32_t) (arg1*arg2);        
                break;
              }

              case UINT64_T: {
    
                uint32_t arg1 = (uint32_t) left.value.integer; 
                uint64_t arg2 = (uint64_t) right.value.integer;
                d.type = UINT64_T;
                d.value.integer = (uint64_t) (arg1*arg2);        
                break;
              }

              case DOUBLE_T: {
    
                uint32_t arg1 = (uint32_t) left.value.integer; 
                double arg2 = (double) right.value.floatingPoint;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

            } //end inner switch

            break;

          } //end outer case


          case UINT64_T: {

            //branch on second factor's type
            switch(right.type) {

              case CHAR_T: {
    
                uint64_t arg1 = (uint64_t) left.value.integer; 
                unsigned char arg2 = (unsigned char) right.value.integer;
                d.type = UINT64_T;
                d.value.integer = (uint64_t) (arg1*arg2);        
                break;
              }

              case INT32_T: {
    
                uint64_t arg1 = (uint64_t) left.value.integer; 
                int32_t arg2 = (int32_t) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case INT64_T: {
    
                uint64_t arg1 = (uint64_t) left.value.integer; 
                int64_t arg2 = (int64_t) right.value.integer;
                d.type = INT64_T;
                d.value.integer = (int64_t) (arg1*arg2);        
                break;
              }

              case UINT32_T: {
    
                uint64_t arg1 = (uint64_t) left.value.integer; 
                uint32_t arg2 = (uint32_t) right.value.integer;
                d.type = UINT64_T;
                d.value.integer = (uint64_t) (arg1*arg2);        
                break;
              }

              case UINT64_T: {
    
                uint64_t arg1 = (uint64_t) left.value.integer; 
                uint64_t arg2 = (uint64_t) right.value.integer;
                d.type = UINT64_T;
                d.value.integer = (uint64_t) (arg1*arg2);        
                break;
              }

              case DOUBLE_T: {
    
                uint64_t arg1 = (uint64_t) left.value.integer; 
                double arg2 = (double) right.value.floatingPoint;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

            } //end inner switch

            break;

          } //end outer case

          case DOUBLE_T: {

            //branch on second factor's type
            switch(right.type) {

              case CHAR_T: {
    
                double arg1 = (double) left.value.floatingPoint;
                unsigned char arg2 = (unsigned char) right.value.integer;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

              case INT32_T: {
    
                double arg1 = (double) left.value.floatingPoint;
                int32_t arg2 = (int32_t) right.value.integer;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

              case INT64_T: {
    
                double arg1 = (double) left.value.floatingPoint;
                int64_t arg2 = (int64_t) right.value.integer;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

              case UINT32_T: {
    
                double arg1 = (double) left.value.floatingPoint;
                uint32_t arg2 = (uint32_t) right.value.integer;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

              case UINT64_T: {
    
                double arg1 = (double) left.value.floatingPoint;
                uint64_t arg2 = (uint64_t) right.value.integer;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

              case DOUBLE_T: {
    
                double arg1 = (double) left.value.floatingPoint;
                double arg2 = (double) right.value.floatingPoint;
                d.type = DOUBLE_T;
                d.value.floatingPoint = (double) (arg1*arg2);        
                break;
              }

            } //end inner switch

            break;

          } //end outer case

          break;
        }

      }
	  
      break;  //end operation case
    }

    ////////////////////////////////
    ///////      Modulus     ///////
    ////////////////////////////////
    
    case MOD_OP: { 

      //branch on first argument's type
      switch(left.type) {

        case CHAR_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = CHAR_T;
              d.value.integer = (unsigned char) (arg1 % arg2);        
              break;
            }

            case INT32_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (arg1 % arg2);        
              break;
            }

            case INT64_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case UINT32_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = UINT32_T;
              d.value.integer = (uint32_t) (arg1 % arg2);        
              break;
            }

            case UINT64_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (arg1 % arg2);        
              break;
            }

            case DOUBLE_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case INT32_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (arg1 % arg2);        
              break;
            }

            case INT32_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (arg1 % arg2);        
              break;
            }

            case INT64_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case UINT32_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (arg1 % arg2);        
              break;
            }

            case UINT64_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case DOUBLE_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case INT64_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case INT32_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case INT64_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case UINT32_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case UINT64_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case DOUBLE_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case UINT32_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = UINT32_T;
              d.value.integer = (uint32_t) (arg1 % arg2);        
              break;
            }

            case INT32_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (arg1 % arg2);        
              break;
            }

            case INT64_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case UINT32_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = UINT32_T;
              d.value.integer = (uint32_t) (arg1 % arg2);        
              break;
            }

            case UINT64_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (arg1 % arg2);        
              break;
            }

            case DOUBLE_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case UINT64_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (arg1 % arg2);        
              break;
            }

            case INT32_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case INT64_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1 % arg2);        
              break;
            }

            case UINT32_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (arg1 % arg2);        
              break;
            }

            case UINT64_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (arg1 % arg2);        
              break;
            }

            case DOUBLE_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case

        case DOUBLE_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);        
              break;
            }

            case INT32_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);        
              break;
            }

            case INT64_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);        
              break;
            }

            case UINT32_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);       
              break;
            }

            case UINT64_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);      
              break;
            }

            case DOUBLE_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) fmod(arg1, arg2);      
              break;
            }

          } //end inner switch

          break;

        } //end outer case

        break;
      }

      break; //end operation case

    }
    
    
    
    case ADD_OP: break;
    
    ////////////////////////////////
    ///////      Subtract     ///////
    ////////////////////////////////
    
    case SUBTRACT_OP: { 

      //branch on first argument's type
      switch(left.type) {

        case CHAR_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = CHAR_T;
              d.value.integer = (unsigned char) (arg1-arg2);        
              break;
            }

            case INT32_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (arg1-arg2);        
              break;
            }

            case INT64_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case UINT32_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = UINT32_T;
              d.value.integer = (uint32_t) (arg1-arg2);        
              break;
            }

            case UINT64_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (arg1-arg2);        
              break;
            }

            case DOUBLE_T: {
  
              unsigned char arg1 = (unsigned char) left.value.integer; 
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case INT32_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (arg1-arg2);        
              break;
            }

            case INT32_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (arg1-arg2);        
              break;
            }

            case INT64_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case UINT32_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (arg1-arg2);        
              break;
            }

            case UINT64_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case DOUBLE_T: {
  
              int32_t arg1 = (int32_t) left.value.integer; 
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case INT64_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case INT32_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case INT64_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case UINT32_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case UINT64_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case DOUBLE_T: {
  
              int64_t arg1 = (int64_t) left.value.integer; 
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case UINT32_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = UINT32_T;
              d.value.integer = (uint32_t) (arg1-arg2);        
              break;
            }

            case INT32_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = INT32_T;
              d.value.integer = (int32_t) (arg1-arg2);        
              break;
            }

            case INT64_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case UINT32_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = UINT32_T;
              d.value.integer = (uint32_t) (arg1-arg2);        
              break;
            }

            case UINT64_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (arg1-arg2);        
              break;
            }

            case DOUBLE_T: {
  
              uint32_t arg1 = (uint32_t) left.value.integer; 
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case


        case UINT64_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (arg1-arg2);        
              break;
            }

            case INT32_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case INT64_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = INT64_T;
              d.value.integer = (int64_t) (arg1-arg2);        
              break;
            }

            case UINT32_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (arg1-arg2);        
              break;
            }

            case UINT64_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = UINT64_T;
              d.value.integer = (uint64_t) (arg1-arg2);        
              break;
            }

            case DOUBLE_T: {
  
              uint64_t arg1 = (uint64_t) left.value.integer; 
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case

        case DOUBLE_T: {

          //branch on second argument's type
          switch(right.type) {

            case CHAR_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              unsigned char arg2 = (unsigned char) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

            case INT32_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              int32_t arg2 = (int32_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

            case INT64_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              int64_t arg2 = (int64_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

            case UINT32_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              uint32_t arg2 = (uint32_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

            case UINT64_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              uint64_t arg2 = (uint64_t) right.value.integer;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

            case DOUBLE_T: {
  
              double arg1 = (double) left.value.floatingPoint;
              double arg2 = (double) right.value.floatingPoint;
              d.type = DOUBLE_T;
              d.value.floatingPoint = (double) (arg1-arg2);        
              break;
            }

          } //end inner switch

          break;

        } //end outer case

        break;
      }

      break; //end operation case
    }

    break;
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

