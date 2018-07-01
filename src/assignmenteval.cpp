#include <iostream>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <string>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "symboltable.h"
#include "casteval.h"

//similar to assignment in statement level, but you return the assigned value
ParseData evaluateAssignmentExpression(AssignmentExpressionNode* node) {

  //get stuff out of the node first
  SymbolTable* symbolTable = node->symbolTable;
  std::string variable = node->variable;
  AbstractExpressionNode* value = node->value;
  
  ParseDataType type = node->evalType;
  ParseData d = value->evaluate();
  
  //update variable value (innermost scope)
  symbolTable->update(variable, castHelper(d, type));
  
  return d;
}