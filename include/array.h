#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <cstdint>
#include "parsetoken.h"
#include "parsenode.h"

//represents array of values of the specified type
struct Array {

	//type of individual element
	ParseDataType subtype;

	//length of array
	uint32_t length;

	//array of values
	ParseData* values;
};

//return final type of array initializer list
ParseDataType arrayListType(std::vector<AbstractExpressionNode*>* initValues);

#endif