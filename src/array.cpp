#include <iostream>
#include <vector>
#include <cstdint>
#include "parsetoken.h"
#include "parsenode.h"
#include "array.h"

//return final type of array initializer list
ParseDataType arrayListType(std::vector<AbstractExpressionNode*>* initValues) {

	//keep track of most significant integer type
	ParseDataType largestIntegerType = UINT8_T;

	//keep track of whether there is a string or double
	bool existsString = false;
	bool existsDouble = false;

	std::vector<AbstractExpressionNode*>::iterator it;
	for(it = initValues->begin(); it != initValues->end(); it++) {

		ParseDataType type = (*it)->evalType;

		if(type == STRING_T)
			existsString = true;
		else if(type == DOUBLE_T)
			existsDouble = true;
		else if(isIntParseDataType(type))
			largestIntegerType = getLargerNumberParseDataType(largestIntegerType, type);
	}

	//if there is a single string, cast everything else to a string
	if(existsString)
		return STRING_T;
	else if(existsDouble)
		return DOUBLE_T;
	else
		return largestIntegerType;
}