#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include "parsetoken.h"

//allocate a char* copy of a string
char* copyString(const char* str);

//allocate a char* subtring
char* copySubstring(const char* str, int32_t start, int32_t pastEnd);

//return ParseData subarray
ParseData* copySubarray(ParseData* arr, int32_t length, int32_t start, int32_t pastEnd);

#endif