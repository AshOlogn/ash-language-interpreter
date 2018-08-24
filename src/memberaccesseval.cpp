#include <iostream>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <string>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "memberaccesseval.h"
#include "array.h"
#include "utils.h"
#include "exceptions.h"

ParseData sliceHelper(ArrayAccessNode* node, ParseData arr, int32_t startIndex, int32_t endIndex) {
  
  ParseData d;
	ParseDataType type = arr.type;
  d.type = type;
  
	if(type == STRING_T) {

		char* str = (char*) arr.value.allocated;
		int32_t len = strlen(str);
		int32_t start = startIndex;
		int32_t pastEnd = endIndex;

		//calculate actual indices (with negative case)
		if(start < 0)
			start += len + 1;

		if(start < 0 || start > len) {
			throw OutOfBoundsException(false, len, startIndex, copyString(node->context), node->startLine, node->endLine);
		}

		if(pastEnd < 0)
			pastEnd += len + 1;

		if(pastEnd < 0 || pastEnd > len) {
			throw OutOfBoundsException(false, len, endIndex, copyString(node->context), node->startLine, node->endLine);
		}
		
		d.value.allocated = copySubstring((char*) arr.value.allocated, start, pastEnd);

	} else if(type == ARRAY_T) {

		Array* array = (Array*) arr.value.allocated;
		int32_t len = (int32_t) array->length;
		int32_t start = startIndex;
		int32_t pastEnd = endIndex;

		//calculate actual indices (with negative case)
		if(start < 0)
			start += len+1;

		if(start < 0 || start > len) {
			throw OutOfBoundsException(true, len, startIndex, copyString(node->context), node->startLine, node->endLine);
		}

		if(pastEnd < 0)
			pastEnd += len+1;

		if(pastEnd < 0 || pastEnd > len) {
			throw OutOfBoundsException(true, len, endIndex, copyString(node->context), node->startLine, node->endLine);
		}
		
		d.value.allocated = copySubarray(array, start, pastEnd);
	}
  
  return d;   
}


ParseData elementHelper(ArrayAccessNode* node, ParseData arr, int32_t index) {
  
  ParseData d;
	ParseDataType type = arr.type;
	
	if(arr.type == STRING_T) {

		d.type = CHAR_T;
		char* str = (char*) arr.value.allocated;
		int32_t len = strlen(str);
		
		int32_t ind = index;
		if(ind < 0)
			ind += len+1;

		//if index is out-of-bounds, throw exception
		if(ind < 0 || ind > len-1) {
			throw OutOfBoundsException(false, len, index, copyString(node->context), node->startLine, node->endLine);
		}

		d.value.integer = str[ind];
		return d;
		
	} else if(type == ARRAY_T) {

		Array* array = (Array*) arr.value.allocated;
		int32_t len = array->length;
		ParseData* values = array->values;

		int32_t ind = index;
		if(ind < 0)
			ind += len; 
		
		//if index is out-of-bounds, throw exception
		if(ind < 0 || ind > len-1) {
			throw OutOfBoundsException(true, len, index, copyString(node->context), node->startLine, node->endLine);
		}

		ParseData val = values[ind];
		return val;
	}

  return d;
}

ParseData evaluateArrayAccess(ArrayAccessNode* node) {
  
	ParseDataType type = node->evalType;
  ParseData array = node->array->evaluate();
  ParseData start = node->start->evaluate();
  ParseData end;
  
  if(node->isSlice)
    end = node->end->evaluate();
  
  if(node->isSlice) {
    return sliceHelper(node, array, (int32_t) start.value.integer, (int32_t) end.value.integer);
  } else {
    return elementHelper(node, array, (int32_t) start.value.integer); 
  }
}
