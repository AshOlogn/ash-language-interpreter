#include <iostream>
#include <cstring>
#include <string>
#include <exception>
#include "parsetoken.h"
#include "exceptions.h"

const char* LexerException::what() const throw() {
  
  std::string str  = "Lexing error on line ";
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
  
  std::string str  = "Parsing error on line ";
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

const char* StaticTypeException::what() const throw() {

  std::string str  = "Type error on line ";
  str.append(std::to_string(lineNumber));
  str.append(":\n\t");
  str.append(context);
  
  if(isBinary) {
    str.append("with elements ");
    str.append(lexeme);
    str.append(" and ");
    str.append(lexeme2);
  } else {
    str.append("with element ");
    str.append(lexeme);     
  }
  
  str.append("\n");
  
  //now append customized message
  str.append("The ");
  str.append(operation);
  str.append(" operation does not take ");
  
  if(isBinary) {
    str.append(toStringParseDataType(arg1));
    str.append(" and ");
    str.append(toStringParseDataType(arg2));
    str.append(" type arguments");
  } else {
    str.append(toStringParseDataType(arg1));
    str.append(" type argument");    
  }
  
  const char* c = str.c_str();
  uint32_t len = strlen(c);
  char* res = new char[len+1];
  strcpy(res, c);
  res[len] = '\0';
  
  return res;
}



