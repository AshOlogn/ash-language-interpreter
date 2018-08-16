#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>
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

	char* copy = new char[std::max(pastEndIndex-startIndex+1, 1)];
	for(int32_t i = startIndex; i < pastEndIndex; i++) {
		copy[i-startIndex] = str[i];
	}

	copy[std::max(pastEndIndex-startIndex, 0)] = '\0';
	return copy;
}

//return ParseData subarray
Array* copySubarray(Array* arr, int32_t startIndex, int32_t pastEndIndex) {

	//extract fields from Array
	ParseData* values = arr->values;

	ParseData* subarrayValues = (ParseData*) malloc(sizeof(ParseData) * std::max(pastEndIndex-startIndex, 1));
	for(int32_t i = startIndex; i < pastEndIndex; i++) {
		subarrayValues[i-startIndex] = values[i];
	}

	//create subarray Array and return it
	Array* subarray = (Array*) malloc(sizeof(Array));
	subarray->length = (uint32_t) std::max(pastEndIndex-startIndex, 0);
	subarray->subtype = arr->subtype;
	subarray->values = subarrayValues;

	return subarray;
}