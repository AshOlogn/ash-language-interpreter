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
#include "array.h"
#include "utils.h"
#include "exceptions.h"

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

ParseData evaluateArrayAssignmentExpression(ArrayAssignmentExpressionNode* node) {

	//extract instance fields
	SymbolTable* symbolTable = node->symbolTable;
	int32_t index = (int32_t) node->index->evaluate().value.integer;

	Array* arr = (Array*) node->array->evaluate().value.allocated;
	int32_t length = (int32_t) arr->length;

	//calculate final index and throw exception if out of bounds
	int32_t finalIndex = (index < 0) ? index + length : index;
	if(finalIndex < 0 || finalIndex > length-1)
		throw OutOfBoundsException(true, length, index, node->context, node->startLine, node->endLine);

	ParseData value = node->value->evaluate();

	//assign array index
	arr->values[finalIndex] = value;

	//return a deep copy of assigned value
	ParseData retValue;
	retValue.type = value.type;

	if(value.type == STRING_T) {
		retValue.value.allocated = (void*) copyString((char*) value.value.allocated);
	} else if(value.type == DOUBLE_T) {
		retValue.value.floatingPoint = value.value.floatingPoint;
	} else {
		retValue.value.integer = value.value.integer;
	}

	return retValue;
}