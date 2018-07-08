#include <iostream>
#include "parsetoken.h"
#include "parsenode.h"
#include "array.h"
#include "arrayeval.h"

ParseData evaluateArrayExpression(ArrayNode* node) {

	//extract instance fields
	bool isInitialized = node->isInitialized;
	ParseDataType subtype = node->subType;
	uint32_t length = (uint32_t) (node->length->evaluate().value.integer);
	AbstractExpressionNode** elements = node->values;

	//construct the array
	Array* arr = (Array*) malloc(sizeof(Array));
	arr->subtype = subtype;
	arr->length = length;
	
	ParseData* values = (ParseData*) malloc(sizeof(ParseData) * length);
	

	//if the array is initialized, fill it accordingly
	if(isInitialized) {
		for(uint32_t i = 0; i < length ; i++) {
			values[i] = elements[i]->evaluate();
		}
	}
	arr->values = values;

	std::cout << "arrayeval address " << (uint64_t) arr->values << std::endl;


	ParseData d;
	d.type = ARRAY_T;
	d.value.allocated = (void*) arr;
	
	return d;
}

