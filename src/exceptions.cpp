#include <iostream>
#include <cstring>
#include <string>
#include <exception>
#include "exceptions.h"

const char* LexerException::what() const throw() {
  
  std::string str  = "Error on line ";
  str.append(std::to_string(lineNumber));
  str.append(":\n\t");
  str.append(context);
  str.append("with token ");
  str.append(lexeme);
  str.append("\n");
  str.append(message);
  
  const char* c = str.c_str();
  uint32_t len = strlen(c);
  char* res = new char[len+1];
  strcpy(res, c);
  res[len] = '\0';
  
  return res;
}

const char* ParseSyntaxException::what() const throw() {
  
  std::string str  = "Error on line ";
  str.append(std::to_string(lineNumber));
  str.append(":\n\t");
  str.append(context);
  str.append("with element ");
  str.append(lexeme);
  str.append("\n");
  str.append(message);
  
  const char* c = str.c_str();
  uint32_t len = strlen(c);
  char* res = new char[len+1];
  strcpy(res, c);
  res[len] = '\0';
  
  return res;
}

