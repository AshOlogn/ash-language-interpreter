#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include "parsetoken.h"
#include "array.h"

//allocate a char* copy of a string
char* copyString(const char* str);

//allocate a char* subtring
char* copySubstring(const char* str, int32_t start, int32_t pastEnd);

//return Array* subarray
Array* copySubarray(Array* arr, int32_t start, int32_t pastEnd);

#endif