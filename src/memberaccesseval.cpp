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

ParseData sliceHelper(ParseDataType type, ParseData arr, int32_t startIndex, int32_t endIndex) {
  
  ParseData d;
  d.type = type;
  
	if(type == STRING_T) {

		char* substring = copySubstring((char*) arr.value.allocated, startIndex, endIndex); 
		d.value.allocated = substring;

	} else if(type == ARRAY_T) {

		Array* array = (Array*) arr.value.allocated;
		ParseData* subarray = copySubarray((ParseData*) array->values, (int32_t) array->length, startIndex, endIndex);
		d.value.allocated = subarray;
	}
  
  return d;   
}


ParseData elementHelper(ParseDataType type, ParseData arr, int32_t index) {
  
  ParseData d;
  
	if(type == STRING_T) {

		d.type = CHAR_T;
		char* str = (char*) arr.value.allocated;
		int32_t len = strlen(str);
		
		int32_t ind = index;
		if(ind < 0)
			ind += len+1;

		d.value.integer = str[ind];
		return d;
		
	} else if(type == ARRAY_T) {

		Array* array = (Array*) arr.value.allocated;
		int32_t len = array->length;
		ParseData* values = array->values;

		int32_t ind = index;
		if(ind < 0)
			ind += len; 

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
    return sliceHelper(type, array, (int32_t) start.value.integer, (int32_t) end.value.integer);
  } else {
    return elementHelper(type, array, (int32_t) start.value.integer); 
  }
}
