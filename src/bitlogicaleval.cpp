#include <cstdint>
#include <cmath>
#include <cstring>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"

//generic helper functions (to keep code concise)
template<typename T1, typename T2>
ParseData bitHelper(T1 n1, T2 n2, ParseDataType finalType, ParseOperatorType op) {

  T1 arg1 = n1;
  T2 arg2 = n2;
  
  ParseData d;
  d.type = finalType;
  
  switch(op) {
    
    case BIT_AND_OP: {
      switch(finalType) {
        case CHAR_T: d.value.integer = (unsigned char) (n1 & n2); break;
        case INT32_T: d.value.integer = (int32_t) (n1 & n2); break;
        case INT64_T: d.value.integer = (int64_t) (n1 & n2); break;
        case UINT32_T: d.value.integer = (uint32_t) (n1 & n2); break;
        case UINT64_T: d.value.integer = (uint64_t) (n1 & n2); break;
      }
      break;
    }

    case BIT_XOR_OP: {
      switch(finalType) {
        case CHAR_T: d.value.integer = (unsigned char) (n1 ^ n2); break;
        case INT32_T: d.value.integer = (int32_t) (n1 ^ n2); break;
        case INT64_T: d.value.integer = (int64_t) (n1 ^ n2); break;
        case UINT32_T: d.value.integer = (uint32_t) (n1 ^ n2); break;
        case UINT64_T: d.value.integer = (uint64_t) (n1 ^ n2); break;
      }
      break;
    }
    
    case BIT_OR_OP: {
      switch(finalType) {
        case CHAR_T: d.value.integer = (unsigned char) (n1 | n2); break;
        case INT32_T: d.value.integer = (int32_t) (n1 | n2); break;
        case INT64_T: d.value.integer = (int64_t) (n1 | n2); break;
        case UINT32_T: d.value.integer = (uint32_t) (n1 | n2); break;
        case UINT64_T: d.value.integer = (uint64_t) (n1 | n2); break;
      }
      break;
    }

		case BIT_LEFT_OP: {
			switch(finalType) {
        case CHAR_T: d.value.integer = (unsigned char) (n1 << n2); break;
        case INT32_T: d.value.integer = (int32_t) (n1 << n2); break;
        case INT64_T: d.value.integer = (int64_t) (n1 << n2); break;
        case UINT32_T: d.value.integer = (uint32_t) (n1 << n2); break;
        case UINT64_T: d.value.integer = (uint64_t) (n1 << n2); break;
      }
      break;
		}

		case BIT_RIGHT_OP: {
			switch(finalType) {
        case CHAR_T: d.value.integer = (unsigned char) (n1 >> n2); break;
        case INT32_T: d.value.integer = (int32_t) (n1 >> n2); break;
        case INT64_T: d.value.integer = (int64_t) (n1 >> n2); break;
        case UINT32_T: d.value.integer = (uint32_t) (n1 >> n2); break;
        case UINT64_T: d.value.integer = (uint64_t) (n1 >> n2); break;
      }
      break;
		}
  }

  return d;
}

ParseData logicHelper(bool n1, bool n2, ParseOperatorType op) {
 
  ParseData d;
  d.type = BOOL_T;
  
  switch(op) {
    case AND_OP: d.value.integer = n1 && n2; break;
    case XOR_OP: d.value.integer = n1 ^ n2; break;
    case OR_OP: d.value.integer = n1 || n2; break;
  }
 
  return d;
}


//actual bitwise operator and logical operator evaluator function
ParseData evaluateBitLogicalExpression(BitLogicalOperatorNode* node) {

  ParseData left = node->leftArg->evaluate();
  ParseData right = node->rightArg->evaluate();
  
  switch(node->operation) {
    
    case BIT_AND_OP: {

      switch(left.type) {

        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return bitHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_AND_OP); 
            case INT32_T: return bitHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case INT64_T: return bitHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case UINT32_T: return bitHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case UINT64_T: return bitHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_AND_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return bitHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_AND_OP); 
            case INT32_T: return bitHelper((int32_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case INT64_T: return bitHelper((int32_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case UINT32_T: return bitHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case UINT64_T: return bitHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_AND_OP);                
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return bitHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_AND_OP); 
            case INT32_T: return bitHelper((int64_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case INT64_T: return bitHelper((int64_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case UINT32_T: return bitHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case UINT64_T: return bitHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_AND_OP); 
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return bitHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_AND_OP); 
            case INT32_T: return bitHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case INT64_T: return bitHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case UINT32_T: return bitHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case UINT64_T: return bitHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_AND_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return bitHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_AND_OP); 
            case INT32_T: return bitHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case INT64_T: return bitHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case UINT32_T: return bitHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_AND_OP); 
            case UINT64_T: return bitHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_AND_OP); 
          }
        }
      }
    }
    
    case BIT_XOR_OP: {

      switch(left.type) {

        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return bitHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_XOR_OP); 
            case INT32_T: return bitHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case INT64_T: return bitHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case UINT32_T: return bitHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case UINT64_T: return bitHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_XOR_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return bitHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_XOR_OP); 
            case INT32_T: return bitHelper((int32_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case INT64_T: return bitHelper((int32_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case UINT32_T: return bitHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case UINT64_T: return bitHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_XOR_OP);                
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return bitHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_XOR_OP); 
            case INT32_T: return bitHelper((int64_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case INT64_T: return bitHelper((int64_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case UINT32_T: return bitHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case UINT64_T: return bitHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_XOR_OP); 
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return bitHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_XOR_OP); 
            case INT32_T: return bitHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case INT64_T: return bitHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case UINT32_T: return bitHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case UINT64_T: return bitHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_XOR_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return bitHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_XOR_OP); 
            case INT32_T: return bitHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case INT64_T: return bitHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case UINT32_T: return bitHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_XOR_OP); 
            case UINT64_T: return bitHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_XOR_OP); 
          }
        }
      }
    }
    
    case BIT_OR_OP: {

      switch(left.type) {

        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return bitHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_OR_OP); 
            case INT32_T: return bitHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case INT64_T: return bitHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case UINT32_T: return bitHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case UINT64_T: return bitHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_OR_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return bitHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_OR_OP); 
            case INT32_T: return bitHelper((int32_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case INT64_T: return bitHelper((int32_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case UINT32_T: return bitHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case UINT64_T: return bitHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_OR_OP);                
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return bitHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_OR_OP); 
            case INT32_T: return bitHelper((int64_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case INT64_T: return bitHelper((int64_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case UINT32_T: return bitHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case UINT64_T: return bitHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_OR_OP); 
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return bitHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_OR_OP); 
            case INT32_T: return bitHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case INT64_T: return bitHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case UINT32_T: return bitHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case UINT64_T: return bitHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_OR_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return bitHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_OR_OP); 
            case INT32_T: return bitHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case INT64_T: return bitHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case UINT32_T: return bitHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_OR_OP); 
            case UINT64_T: return bitHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_OR_OP); 
          }
        }
      }
    }

		case BIT_LEFT_OP: {

      switch(left.type) {

        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return bitHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case INT32_T: return bitHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case INT64_T: return bitHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case UINT32_T: return bitHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case UINT64_T: return bitHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_LEFT_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return bitHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case INT32_T: return bitHelper((int32_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case INT64_T: return bitHelper((int32_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case UINT32_T: return bitHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case UINT64_T: return bitHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_LEFT_OP);                
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return bitHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case INT32_T: return bitHelper((int64_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case INT64_T: return bitHelper((int64_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case UINT32_T: return bitHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case UINT64_T: return bitHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return bitHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case INT32_T: return bitHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case INT64_T: return bitHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case UINT32_T: return bitHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case UINT64_T: return bitHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_LEFT_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return bitHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case INT32_T: return bitHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case INT64_T: return bitHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case UINT32_T: return bitHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
            case UINT64_T: return bitHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_LEFT_OP); 
          }
        }
      }
    }

		case BIT_RIGHT_OP: {

      switch(left.type) {

        case CHAR_T: {
          
          switch(right.type) {
            case CHAR_T: return bitHelper((unsigned char) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case INT32_T: return bitHelper((unsigned char) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case INT64_T: return bitHelper((unsigned char) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case UINT32_T: return bitHelper((unsigned char) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case UINT64_T: return bitHelper((unsigned char) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_RIGHT_OP);  
          }
        }    

        case INT32_T: {
          
          switch(right.type) {
            case CHAR_T: return bitHelper((int32_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case INT32_T: return bitHelper((int32_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case INT64_T: return bitHelper((int32_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case UINT32_T: return bitHelper((int32_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case UINT64_T: return bitHelper((int32_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_RIGHT_OP);                
          }
        }

        case INT64_T: {

          switch(right.type) {
            case CHAR_T: return bitHelper((int64_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case INT32_T: return bitHelper((int64_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case INT64_T: return bitHelper((int64_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case UINT32_T: return bitHelper((int64_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case UINT64_T: return bitHelper((int64_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
          }    
        }

        case UINT32_T: {

          switch(right.type) {            
            case CHAR_T: return bitHelper((uint32_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case INT32_T: return bitHelper((uint32_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case INT64_T: return bitHelper((uint32_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case UINT32_T: return bitHelper((uint32_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case UINT64_T: return bitHelper((uint32_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_RIGHT_OP);   
          }
        }

        case UINT64_T: {

          switch(right.type) {
            case CHAR_T: return bitHelper((uint64_t) left.value.integer, (unsigned char) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case INT32_T: return bitHelper((uint64_t) left.value.integer, (int32_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case INT64_T: return bitHelper((uint64_t) left.value.integer, (int64_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case UINT32_T: return bitHelper((uint64_t) left.value.integer, (uint32_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
            case UINT64_T: return bitHelper((uint64_t) left.value.integer, (uint64_t) right.value.integer, node->evalType, BIT_RIGHT_OP); 
          }
        }
      }
    }
    
    case AND_OP: return logicHelper((bool) left.value.integer, (bool) right.value.integer, AND_OP); 
    case XOR_OP: return logicHelper((bool) left.value.integer, (bool) right.value.integer, XOR_OP); 
    case OR_OP: return logicHelper((bool) left.value.integer, (bool) right.value.integer, OR_OP); 
  }

  ParseData dummy;
  return dummy;

}


