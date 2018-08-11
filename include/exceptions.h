#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <cstdint>
#include <string>
#include <exception>
#include "parsetoken.h"


//accessing string or array out of bounds
class OutOfBoundsException : public std::exception {

	public:
		char* context;
		bool isArray;
		int32_t length;
		int32_t index;
		uint32_t startLine;
		uint32_t endLine;

		OutOfBoundsException(bool isArr, int32_t len, int32_t ind, char* con, uint32_t startLin, uint32_t endLin)
			: isArray{isArr}, length{len}, index{ind}, context{con}, startLine{startLin}, endLine{endLin} {}
		
		const char* what() const throw ();
};


#endif