#include <cstdint>
#include <cmath>
#include <cstring>
#include <string>
#include "exceptions.h"
#include "symboltable.h"
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


    case POSTFIX_INC_OP: {
      
      VariableNode* varNode = dynamic_cast<VariableNode*>(node->leftArg);

      //get variable name
      const char* constVariableName = varNode->variable.c_str();
			uint32_t len = strlen(constVariableName);
			char* variableName = new char[len+1];
			variableName[0] = '\0';
			strcpy(variableName, constVariableName);
			variableName[len] = '\0';

      //get variable type
      ParseDataType varType = varNode->evalType;

      switch(arg.type) {
        
        case CHAR_T: {
          unsigned char val = (unsigned char) arg.value.integer;
          d.value.integer = val;
          break;
        }
        
        case INT32_T: {
          int32_t val = (int32_t) arg.value.integer;
          d.value.integer = val;
          break;
        }
        
        case INT64_T: {
          int64_t val = (int64_t) arg.value.integer;
          d.value.integer = val;
          break;
        }
        
        case UINT32_T:  {
          uint32_t val = (uint32_t) arg.value.integer;
          d.value.integer = val;
          break;
        }
        
        case UINT64_T: {
          uint64_t val = (uint64_t) arg.value.integer;
          d.value.integer = val;
          break;
        }
        
        case DOUBLE_T: {
          double val = (double) arg.value.floatingPoint;
          d.value.floatingPoint = val;
          break;
        }
        
      }

      //update value in symbol table
      ParseData update;
      update.type = d.type;
      if(d.type == DOUBLE_T) {
        update.value.floatingPoint = d.value.floatingPoint + 1.0;
      } else {
        update.value.integer = d.value.integer + 1;
      }

      SymbolTable* symbolTable = node->symbolTable;
      symbolTable->update(variableName, update);

      //return original value
      return d;
    }

    case POSTFIX_DEC_OP: {
      
      VariableNode* varNode = dynamic_cast<VariableNode*>(node->leftArg);

      //get variable name
      const char* constVariableName = varNode->variable.c_str();
			uint32_t len = strlen(constVariableName);
			char* variableName = new char[len+1];
			variableName[0] = '\0';
			strcpy(variableName, constVariableName);
			variableName[len] = '\0';

      //get variable type
      ParseDataType varType = varNode->evalType;

      switch(arg.type) {
        
        case CHAR_T: {
          unsigned char val = (unsigned char) arg.value.integer;
          d.value.integer = val;
          break;
        }
        
        case INT32_T: {
          int32_t val = (int32_t) arg.value.integer;
          d.value.integer = val;
          break;
        }
        
        case INT64_T: {
          int64_t val = (int64_t) arg.value.integer;
          d.value.integer = val;
          break;
        }
        
        case UINT32_T:  {
          uint32_t val = (uint32_t) arg.value.integer;
          d.value.integer = val;
          break;
        }
        
        case UINT64_T: {
          uint64_t val = (uint64_t) arg.value.integer;
          d.value.integer = val;
          break;
        }
        
        case DOUBLE_T: {
          double val = (double) arg.value.floatingPoint;
          d.value.floatingPoint = val;
          break;
        }
        
      }

      //update value in symbol table
      ParseData update;
      update.type = d.type;
      if(d.type == DOUBLE_T) {
        update.value.floatingPoint = d.value.floatingPoint - 1.0;
      } else {
        update.value.integer = d.value.integer - 1;
      }

      SymbolTable* symbolTable = node->symbolTable;
      symbolTable->update(variableName, update);

      //return original value
      return d;
    }

    case PREFIX_INC_OP: {
      
      VariableNode* varNode = dynamic_cast<VariableNode*>(node->leftArg);

      //get variable name
      const char* constVariableName = varNode->variable.c_str();
			uint32_t len = strlen(constVariableName);
			char* variableName = new char[len+1];
			variableName[0] = '\0';
			strcpy(variableName, constVariableName);
			variableName[len] = '\0';

      //get variable type
      ParseDataType varType = varNode->evalType;

      switch(arg.type) {
        
        case CHAR_T: {
          unsigned char val = (unsigned char) (arg.value.integer + 1);
          d.value.integer = val;
          break;
        }
        
        case INT32_T: {
          int32_t val = (int32_t) (arg.value.integer + 1);
          d.value.integer = val;
          break;
        }
        
        case INT64_T: {
          int64_t val = (int64_t) (arg.value.integer + 1);
          d.value.integer = val;
          break;
        }
        
        case UINT32_T:  {
          uint32_t val = (uint32_t) (arg.value.integer + 1);
          d.value.integer = val;
          break;
        }
        
        case UINT64_T: {
          uint64_t val = (uint64_t) (arg.value.integer + 1);
          d.value.integer = val;
          break;
        }
        
        case DOUBLE_T: {
          double val = (double) (arg.value.floatingPoint + 1.0);
          d.value.floatingPoint = val;
          break;
        }
        
      }

      //update value in symbol table
      SymbolTable* symbolTable = node->symbolTable;
      symbolTable->update(variableName, d);

      //return original value
      return d;
    }

    case PREFIX_DEC_OP: {
      
      VariableNode* varNode = dynamic_cast<VariableNode*>(node->leftArg);

      //get variable name
      const char* constVariableName = varNode->variable.c_str();
			uint32_t len = strlen(constVariableName);
			char* variableName = new char[len+1];
			variableName[0] = '\0';
			strcpy(variableName, constVariableName);
			variableName[len] = '\0';

      //get variable type
      ParseDataType varType = varNode->evalType;

      switch(arg.type) {
        
        case CHAR_T: {
          unsigned char val = (unsigned char) (arg.value.integer - 1);
          d.value.integer = val;
          break;
        }
        
        case INT32_T: {
          int32_t val = (int32_t) (arg.value.integer - 1);
          d.value.integer = val;
          break;
        }
        
        case INT64_T: {
          int64_t val = (int64_t) (arg.value.integer - 1);
          d.value.integer = val;
          break;
        }
        
        case UINT32_T:  {
          uint32_t val = (uint32_t) (arg.value.integer - 1);
          d.value.integer = val;
          break;
        }
        
        case UINT64_T: {
          uint64_t val = (uint64_t) (arg.value.integer - 1);
          d.value.integer = val;
          break;
        }
        
        case DOUBLE_T: {
          double val = (double) (arg.value.floatingPoint - 1.0);
          d.value.floatingPoint = val;
          break;
        }
      }

      SymbolTable* symbolTable = node->symbolTable;
      symbolTable->update(variableName, d);

      //return original value
      return d;
    }
  }
  
  return d;
}