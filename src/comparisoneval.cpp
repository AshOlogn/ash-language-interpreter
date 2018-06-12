#include <iostream>
#include <cstdint>
#include <cmath>
#include <cstring>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"


//generic helper methods (to keep source code concise)

template<typename T1, typename T2>
ParseData inequalityHelper(T1 n1, T2 n2, ParseOperatorType op) {
  
  T1 arg1 = n1;
  T2 arg2 = n2;
  
  ParseData d;
  d.type = BOOL_T;
  
  switch(op) { 
    case GREATER_OP: d.value.integer = n1 > n2; break;
    case GREATER_EQ_OP: d.value.integer = n1 >= n2; break;
    case LESS_OP: d.value.integer = n1 < n2; break;
    case LESS_EQ_OP: d.value.integer = n1 <= n2; break;
  }

  return d;
}

template<>
ParseData inequalityHelper<char*, char*>(char* str1, char* str2, ParseOperatorType op) {
 
  char* arg1 = str1;
  char* arg2 = str2;
  
  ParseData d;
  d.type = BOOL_T;
  int32_t res = strcmp(str1, str2);
  
  switch(op) { 
    case GREATER_OP: d.value.integer = res > 0; break;
    case GREATER_EQ_OP: d.value.integer = res >= 0; break;
    case LESS_OP: d.value.integer = res < 0; break;
    case LESS_EQ_OP: d.value.integer = res <= 0; break;
  }
  
  return d; 
}

template<typename T1, typename T2>
ParseData equalityHelper(T1 n1, T2 n2, ParseOperatorType op) {
  
  T1 arg1 = n1;
  T2 arg2 = n2;
  
  ParseData d;
  d.type = BOOL_T;
  
  switch(op) {
    case EQ_EQ_OP: d.value.integer = n1 == n2; break;
    case NOT_EQ_OP: d.value.integer = n1 != n2; break;
  }

  return d;
}

template<>
ParseData equalityHelper<char*, char*>(char* str1, char* str2, ParseOperatorType op) {
 
  char* arg1 = str1;
  char* arg2 = str2;
  
  ParseData d;
  d.type = BOOL_T;
  int32_t res = strcmp(str1, str2);
  
  switch(op) { 
    case EQ_EQ_OP: d.value.integer = res == 0; break;
    case NOT_EQ_OP: d.value.integer = res != 0; break;
  }
  
  return d; 
}


//actual comparison evaluator function
ParseData evaluateComparisonExpression(ComparisonOperatorNode* node) {

  //recursively evaluate left and right subtrees
  ParseData left = node->leftArg->evaluate();
  ParseData right = node->rightArg->evaluate();
  
  
  switch(node->operation) {
    
      
    case GREATER_OP: {

      switch(left.type) {

        case STRING_T: return inequalityHelper((char*) left.value.allocated, (char*) right.value.allocated, GREATER_OP);
      
        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, GREATER_OP); 
            case INT32_T: return inequalityHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, GREATER_OP); 
            case INT64_T: return inequalityHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, GREATER_OP); 
            case UINT32_T: return inequalityHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, GREATER_OP); 
            case UINT64_T: return inequalityHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, GREATER_OP); 
            case DOUBLE_T: return inequalityHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, GREATER_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, GREATER_OP); 
            case INT32_T: return inequalityHelper((int32_t) left.value.integer, (int32_t) right.value.integer, GREATER_OP); 
            case INT64_T: return inequalityHelper((int32_t) left.value.integer, (int64_t) right.value.integer, GREATER_OP); 
            case UINT32_T: return inequalityHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, GREATER_OP); 
            case UINT64_T: return inequalityHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, GREATER_OP); 
            case DOUBLE_T: return inequalityHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, GREATER_OP);               
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return inequalityHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, GREATER_OP); 
            case INT32_T: return inequalityHelper((int64_t) left.value.integer, (int32_t) right.value.integer, GREATER_OP); 
            case INT64_T: return inequalityHelper((int64_t) left.value.integer, (int64_t) right.value.integer, GREATER_OP); 
            case UINT32_T: return inequalityHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, GREATER_OP); 
            case UINT64_T: return inequalityHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, GREATER_OP); 
            case DOUBLE_T: return inequalityHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, GREATER_OP);  
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return inequalityHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, GREATER_OP); 
            case INT32_T: return inequalityHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, GREATER_OP); 
            case INT64_T: return inequalityHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, GREATER_OP); 
            case UINT32_T: return inequalityHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, GREATER_OP); 
            case UINT64_T: return inequalityHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, GREATER_OP); 
            case DOUBLE_T: return inequalityHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, GREATER_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return inequalityHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, GREATER_OP); 
            case INT32_T: return inequalityHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, GREATER_OP); 
            case INT64_T: return inequalityHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, GREATER_OP); 
            case UINT32_T: return inequalityHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, GREATER_OP); 
            case UINT64_T: return inequalityHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, GREATER_OP); 
            case DOUBLE_T: return inequalityHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, GREATER_OP);  
          }
        }

        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, GREATER_OP); 
            case INT32_T: return inequalityHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, GREATER_OP); 
            case INT64_T: return inequalityHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, GREATER_OP); 
            case UINT32_T: return inequalityHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, GREATER_OP); 
            case UINT64_T: return inequalityHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, GREATER_OP); 
            case DOUBLE_T: return inequalityHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, GREATER_OP);  
          }
        }
      }
    }

    case GREATER_EQ_OP: {

      switch(left.type) {

        case STRING_T: return inequalityHelper((char*) left.value.allocated, (char*) right.value.allocated, GREATER_EQ_OP);
      
        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, GREATER_EQ_OP); 
            case INT32_T: return inequalityHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, GREATER_EQ_OP); 
            case INT64_T: return inequalityHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, GREATER_EQ_OP); 
            case UINT32_T: return inequalityHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, GREATER_EQ_OP); 
            case UINT64_T: return inequalityHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, GREATER_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, GREATER_EQ_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, GREATER_EQ_OP); 
            case INT32_T: return inequalityHelper((int32_t) left.value.integer, (int32_t) right.value.integer, GREATER_EQ_OP); 
            case INT64_T: return inequalityHelper((int32_t) left.value.integer, (int64_t) right.value.integer, GREATER_EQ_OP); 
            case UINT32_T: return inequalityHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, GREATER_EQ_OP); 
            case UINT64_T: return inequalityHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, GREATER_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, GREATER_EQ_OP);               
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return inequalityHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, GREATER_EQ_OP); 
            case INT32_T: return inequalityHelper((int64_t) left.value.integer, (int32_t) right.value.integer, GREATER_EQ_OP); 
            case INT64_T: return inequalityHelper((int64_t) left.value.integer, (int64_t) right.value.integer, GREATER_EQ_OP); 
            case UINT32_T: return inequalityHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, GREATER_EQ_OP); 
            case UINT64_T: return inequalityHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, GREATER_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, GREATER_EQ_OP);  
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return inequalityHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, GREATER_EQ_OP); 
            case INT32_T: return inequalityHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, GREATER_EQ_OP); 
            case INT64_T: return inequalityHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, GREATER_EQ_OP); 
            case UINT32_T: return inequalityHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, GREATER_EQ_OP); 
            case UINT64_T: return inequalityHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, GREATER_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, GREATER_EQ_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return inequalityHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, GREATER_EQ_OP); 
            case INT32_T: return inequalityHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, GREATER_EQ_OP); 
            case INT64_T: return inequalityHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, GREATER_EQ_OP); 
            case UINT32_T: return inequalityHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, GREATER_EQ_OP); 
            case UINT64_T: return inequalityHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, GREATER_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, GREATER_EQ_OP);  
          }
        }

        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, GREATER_EQ_OP); 
            case INT32_T: return inequalityHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, GREATER_EQ_OP); 
            case INT64_T: return inequalityHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, GREATER_EQ_OP); 
            case UINT32_T: return inequalityHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, GREATER_EQ_OP); 
            case UINT64_T: return inequalityHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, GREATER_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, GREATER_EQ_OP);  
          }
        }
      }
    }

    case LESS_OP: {

      switch(left.type) {

        case STRING_T: return inequalityHelper((char*) left.value.allocated, (char*) right.value.allocated, LESS_OP);
      
        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, LESS_OP); 
            case INT32_T: return inequalityHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, LESS_OP); 
            case INT64_T: return inequalityHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, LESS_OP); 
            case UINT32_T: return inequalityHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, LESS_OP); 
            case UINT64_T: return inequalityHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, LESS_OP); 
            case DOUBLE_T: return inequalityHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, LESS_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, LESS_OP); 
            case INT32_T: return inequalityHelper((int32_t) left.value.integer, (int32_t) right.value.integer, LESS_OP); 
            case INT64_T: return inequalityHelper((int32_t) left.value.integer, (int64_t) right.value.integer, LESS_OP); 
            case UINT32_T: return inequalityHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, LESS_OP); 
            case UINT64_T: return inequalityHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, LESS_OP); 
            case DOUBLE_T: return inequalityHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, LESS_OP);               
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return inequalityHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, LESS_OP); 
            case INT32_T: return inequalityHelper((int64_t) left.value.integer, (int32_t) right.value.integer, LESS_OP); 
            case INT64_T: return inequalityHelper((int64_t) left.value.integer, (int64_t) right.value.integer, LESS_OP); 
            case UINT32_T: return inequalityHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, LESS_OP); 
            case UINT64_T: return inequalityHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, LESS_OP); 
            case DOUBLE_T: return inequalityHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, LESS_OP);  
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return inequalityHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, LESS_OP); 
            case INT32_T: return inequalityHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, LESS_OP); 
            case INT64_T: return inequalityHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, LESS_OP); 
            case UINT32_T: return inequalityHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, LESS_OP); 
            case UINT64_T: return inequalityHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, LESS_OP); 
            case DOUBLE_T: return inequalityHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, LESS_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return inequalityHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, LESS_OP); 
            case INT32_T: return inequalityHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, LESS_OP); 
            case INT64_T: return inequalityHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, LESS_OP); 
            case UINT32_T: return inequalityHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, LESS_OP); 
            case UINT64_T: return inequalityHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, LESS_OP); 
            case DOUBLE_T: return inequalityHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, LESS_OP);  
          }
        }

        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, LESS_OP); 
            case INT32_T: return inequalityHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, LESS_OP); 
            case INT64_T: return inequalityHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, LESS_OP); 
            case UINT32_T: return inequalityHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, LESS_OP); 
            case UINT64_T: return inequalityHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, LESS_OP); 
            case DOUBLE_T: return inequalityHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, LESS_OP);  
          }
        }
      }
    }

    case LESS_EQ_OP: {

      switch(left.type) {

        case STRING_T: return inequalityHelper((char*) left.value.allocated, (char*) right.value.allocated, LESS_EQ_OP);
      
        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, LESS_EQ_OP); 
            case INT32_T: return inequalityHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, LESS_EQ_OP); 
            case INT64_T: return inequalityHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, LESS_EQ_OP); 
            case UINT32_T: return inequalityHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, LESS_EQ_OP); 
            case UINT64_T: return inequalityHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, LESS_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, LESS_EQ_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, LESS_EQ_OP); 
            case INT32_T: return inequalityHelper((int32_t) left.value.integer, (int32_t) right.value.integer, LESS_EQ_OP); 
            case INT64_T: return inequalityHelper((int32_t) left.value.integer, (int64_t) right.value.integer, LESS_EQ_OP); 
            case UINT32_T: return inequalityHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, LESS_EQ_OP); 
            case UINT64_T: return inequalityHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, LESS_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, LESS_EQ_OP);               
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return inequalityHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, LESS_EQ_OP); 
            case INT32_T: return inequalityHelper((int64_t) left.value.integer, (int32_t) right.value.integer, LESS_EQ_OP); 
            case INT64_T: return inequalityHelper((int64_t) left.value.integer, (int64_t) right.value.integer, LESS_EQ_OP); 
            case UINT32_T: return inequalityHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, LESS_EQ_OP); 
            case UINT64_T: return inequalityHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, LESS_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, LESS_EQ_OP);  
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return inequalityHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, LESS_EQ_OP); 
            case INT32_T: return inequalityHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, LESS_EQ_OP); 
            case INT64_T: return inequalityHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, LESS_EQ_OP); 
            case UINT32_T: return inequalityHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, LESS_EQ_OP); 
            case UINT64_T: return inequalityHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, LESS_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, LESS_EQ_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return inequalityHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, LESS_EQ_OP); 
            case INT32_T: return inequalityHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, LESS_EQ_OP); 
            case INT64_T: return inequalityHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, LESS_EQ_OP); 
            case UINT32_T: return inequalityHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, LESS_EQ_OP); 
            case UINT64_T: return inequalityHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, LESS_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, LESS_EQ_OP);  
          }
        }

        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return inequalityHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, LESS_EQ_OP); 
            case INT32_T: return inequalityHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, LESS_EQ_OP); 
            case INT64_T: return inequalityHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, LESS_EQ_OP); 
            case UINT32_T: return inequalityHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, LESS_EQ_OP); 
            case UINT64_T: return inequalityHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, LESS_EQ_OP); 
            case DOUBLE_T: return inequalityHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, LESS_EQ_OP);  
          }
        }
      }
    }

    case EQ_EQ_OP: {

      switch(left.type) {

        case STRING_T: return equalityHelper((char*) left.value.allocated, (char*) right.value.allocated, EQ_EQ_OP);
      
        case BOOL_T: return equalityHelper((bool) left.value.integer, (bool) right.value.integer, EQ_EQ_OP);
      
        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return equalityHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, EQ_EQ_OP); 
            case INT32_T: return equalityHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, EQ_EQ_OP); 
            case INT64_T: return equalityHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, EQ_EQ_OP); 
            case UINT32_T: return equalityHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, EQ_EQ_OP); 
            case UINT64_T: return equalityHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, EQ_EQ_OP); 
            case DOUBLE_T: return equalityHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, EQ_EQ_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return equalityHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, EQ_EQ_OP); 
            case INT32_T: return equalityHelper((int32_t) left.value.integer, (int32_t) right.value.integer, EQ_EQ_OP); 
            case INT64_T: return equalityHelper((int32_t) left.value.integer, (int64_t) right.value.integer, EQ_EQ_OP); 
            case UINT32_T: return equalityHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, EQ_EQ_OP); 
            case UINT64_T: return equalityHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, EQ_EQ_OP); 
            case DOUBLE_T: return equalityHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, EQ_EQ_OP);               
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return equalityHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, EQ_EQ_OP); 
            case INT32_T: return equalityHelper((int64_t) left.value.integer, (int32_t) right.value.integer, EQ_EQ_OP); 
            case INT64_T: return equalityHelper((int64_t) left.value.integer, (int64_t) right.value.integer, EQ_EQ_OP); 
            case UINT32_T: return equalityHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, EQ_EQ_OP); 
            case UINT64_T: return equalityHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, EQ_EQ_OP); 
            case DOUBLE_T: return equalityHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, EQ_EQ_OP);  
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return equalityHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, EQ_EQ_OP); 
            case INT32_T: return equalityHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, EQ_EQ_OP); 
            case INT64_T: return equalityHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, EQ_EQ_OP); 
            case UINT32_T: return equalityHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, EQ_EQ_OP); 
            case UINT64_T: return equalityHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, EQ_EQ_OP); 
            case DOUBLE_T: return equalityHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, EQ_EQ_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return equalityHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, EQ_EQ_OP); 
            case INT32_T: return equalityHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, EQ_EQ_OP); 
            case INT64_T: return equalityHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, EQ_EQ_OP); 
            case UINT32_T: return equalityHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, EQ_EQ_OP); 
            case UINT64_T: return equalityHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, EQ_EQ_OP); 
            case DOUBLE_T: return equalityHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, EQ_EQ_OP);  
          }
        }

        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return equalityHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, EQ_EQ_OP); 
            case INT32_T: return equalityHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, EQ_EQ_OP); 
            case INT64_T: return equalityHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, EQ_EQ_OP); 
            case UINT32_T: return equalityHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, EQ_EQ_OP); 
            case UINT64_T: return equalityHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, EQ_EQ_OP); 
            case DOUBLE_T: return equalityHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, EQ_EQ_OP);  
          }
        }
      }
    }    
    
    case NOT_EQ_OP: {

      switch(left.type) {

        case STRING_T: return equalityHelper((char*) left.value.allocated, (char*) right.value.allocated, NOT_EQ_OP);
      
        case BOOL_T: return equalityHelper((bool) left.value.integer, (bool) right.value.integer, NOT_EQ_OP);
      
        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return equalityHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, NOT_EQ_OP); 
            case INT32_T: return equalityHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, NOT_EQ_OP); 
            case INT64_T: return equalityHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, NOT_EQ_OP); 
            case UINT32_T: return equalityHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, NOT_EQ_OP); 
            case UINT64_T: return equalityHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, NOT_EQ_OP); 
            case DOUBLE_T: return equalityHelper((unsigned char) left.value.integer, (double) right.value.floatingPoint, NOT_EQ_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return equalityHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, NOT_EQ_OP); 
            case INT32_T: return equalityHelper((int32_t) left.value.integer, (int32_t) right.value.integer, NOT_EQ_OP); 
            case INT64_T: return equalityHelper((int32_t) left.value.integer, (int64_t) right.value.integer, NOT_EQ_OP); 
            case UINT32_T: return equalityHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, NOT_EQ_OP); 
            case UINT64_T: return equalityHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, NOT_EQ_OP); 
            case DOUBLE_T: return equalityHelper((int32_t) left.value.integer, (double) right.value.floatingPoint, NOT_EQ_OP);               
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return equalityHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, NOT_EQ_OP); 
            case INT32_T: return equalityHelper((int64_t) left.value.integer, (int32_t) right.value.integer, NOT_EQ_OP); 
            case INT64_T: return equalityHelper((int64_t) left.value.integer, (int64_t) right.value.integer, NOT_EQ_OP); 
            case UINT32_T: return equalityHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, NOT_EQ_OP); 
            case UINT64_T: return equalityHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, NOT_EQ_OP); 
            case DOUBLE_T: return equalityHelper((int64_t) left.value.integer, (double) right.value.floatingPoint, NOT_EQ_OP);  
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return equalityHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, NOT_EQ_OP); 
            case INT32_T: return equalityHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, NOT_EQ_OP); 
            case INT64_T: return equalityHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, NOT_EQ_OP); 
            case UINT32_T: return equalityHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, NOT_EQ_OP); 
            case UINT64_T: return equalityHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, NOT_EQ_OP); 
            case DOUBLE_T: return equalityHelper((uint32_t) left.value.integer, (double) right.value.floatingPoint, NOT_EQ_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return equalityHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, NOT_EQ_OP); 
            case INT32_T: return equalityHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, NOT_EQ_OP); 
            case INT64_T: return equalityHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, NOT_EQ_OP); 
            case UINT32_T: return equalityHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, NOT_EQ_OP); 
            case UINT64_T: return equalityHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, NOT_EQ_OP); 
            case DOUBLE_T: return equalityHelper((uint64_t) left.value.integer, (double) right.value.floatingPoint, NOT_EQ_OP);  
          }
        }

        case DOUBLE_T: {
          
          switch(right.type) {
            case CHAR_T: return equalityHelper((double) left.value.floatingPoint, (unsigned char) right.value.integer, NOT_EQ_OP); 
            case INT32_T: return equalityHelper((double) left.value.floatingPoint, (int32_t) right.value.integer, NOT_EQ_OP); 
            case INT64_T: return equalityHelper((double) left.value.floatingPoint, (int64_t) right.value.integer, NOT_EQ_OP); 
            case UINT32_T: return equalityHelper((double) left.value.floatingPoint, (uint32_t) right.value.integer, NOT_EQ_OP); 
            case UINT64_T: return equalityHelper((double) left.value.floatingPoint, (uint64_t) right.value.integer, NOT_EQ_OP); 
            case DOUBLE_T: return equalityHelper((double) left.value.floatingPoint, (double) right.value.floatingPoint, NOT_EQ_OP);  
          }
        }
      }
    }     
      
  }
  
  
  ParseData dummy;
  return dummy;
}

