#ifndef ARRAY_H
#define ARRAY_H

#include <cstdint>
#include "parsetoken.h"
#include "parsenode.h"

//represents array of values of the specified type
struct Array {
	ParseDataType subtype;
	uint32_t length;
	ParseData* values;
};

#endif