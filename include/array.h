#ifndef ARRAY_H
#define ARRAY_H

#include <cstdint>
#include "parsetoken.h"

//represents array of values of the specified type
struct Array {
	ParseDataType subtype;
	uint32_t length;
	void* values;
};

#endif