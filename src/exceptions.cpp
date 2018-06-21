#include <cstring>
#include <string>
#include <exception>
#include "exceptions.h"

const char* LexerException::what() const throw() {
  
  std::string str  = "Error on line ";
  str.append(std::to_string(lineNumber));
  str.append(":\n\t");
  str.append(context);
  str.append(message);
  return str.c_str();
}

