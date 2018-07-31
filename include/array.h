#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <cstdint>
#include "parsetoken.h"
#include "parsenode.h"

//represents array of values of the specified type
struct Array {
	ParseDataType subtype;
	uint32_t length;
	ParseData* values;
};

//return final type of array initializer list
ParseDataType arrayListType(std::vector<AbstractExpressionNode*>* initValues);

#endif