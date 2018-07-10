#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include "array.h"
#include "utils.h"

//allocate a char* copy of a string
char* copyString(const char* str) {

	uint32_t len = strlen(str);
	char* copy = new char[len+1];
	copy[0] = '\0';

	strcpy(copy, str);
	copy[len] = '\0';

	return copy;
}

//allocate a char* subtring
char* copySubstring(const char* str, int32_t startIndex, int32_t pastEndIndex) {

	int32_t len = strlen(str);
	int32_t start = startIndex;
	int32_t pastEnd = pastEndIndex;

	//take into account negative indices
	if(start < 0)
		start += len + 1;
	
	if(pastEnd < 0)
		pastEnd += len + 1;

	if(pastEnd <= start) {
		char* copy = new char[1];
		copy[0] = '\0';
		return copy;
	}

	char* copy = new char[pastEnd-start+1];
	for(int32_t i = start; i < pastEnd; i++) {
		copy[i-start] = str[i];
	}
	copy[pastEnd-start] = '\0';

	return copy;
}

//return ParseData subarray
Array* copySubarray(Array* arr, int32_t startIndex, int32_t pastEndIndex) {

	//extract fields from Array
	int32_t length = (int32_t) arr->length;
	ParseData* values = arr->values;

	int32_t start = startIndex;
	int32_t pastEnd = pastEndIndex;

	//take into account negative indices
	if(start < 0)
		start += length + 1;
	
	if(pastEnd < 0)
		pastEnd += length + 1;

	ParseData* subarrayValues = (ParseData*) malloc(sizeof(ParseData) * (pastEnd-start));
	for(int32_t i = start; i < pastEnd; i++) {
		subarrayValues[i-start] = values[i];
	}

	//create subarray Array and return it
	Array* subarray = (Array*) malloc(sizeof(Array));
	subarray->length = (uint32_t) (pastEnd-start);
	subarray->subtype = arr->subtype;
	subarray->values = subarrayValues;

	return subarray;
}