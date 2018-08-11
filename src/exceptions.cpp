#include <iostream>
#include <cstring>
#include <string>
#include <exception>
#include "utils.h"
#include "parsetoken.h"
#include "exceptions.h"

const char* OutOfBoundsException::what() const throw() {

	std::string str = "";

	if(startLine == endLine) {
		str.append("OutOfBoundsException on line ");
		str.append(std::to_string(startLine));
	} else {
		str.append("OutOfBoundsException from line ");
		str.append(std::to_string(startLine));
		str.append(" to ");
		str.append(std::to_string(endLine));
	}

	str.append(":\n\t");
	str.append(context);

	//error message
	str.append("The index ");
	str.append(std::to_string(index));
	str.append(" is out of bounds in ");
	str.append((isArray) ? "array" : "string");
	str.append(" of length ");
	str.append(std::to_string(length));

	return copyString(str.c_str());
}