#include <iostream>
#include <cstring>
#include <string>
#include <exception>
#include "parsetoken.h"
#include "exceptions.h"

const char* LexerException::what() const throw() {
  
  std::string str  = "LexerException on line ";
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
  
  std::string str = "";
  
  if(startLineNumber == endLineNumber) {
    str.append("ParseSyntaxException error on line ");
    str.append(std::to_string(startLineNumber));
  } else {
    str.append("ParseSyntaxException error from line ");
    str.append(std::to_string(startLineNumber));
    str.append(" to ");
    str.append(std::to_string(endLineNumber));
  }
  
  str.append(":\n\t");
  str.append(context);

  if(lexeme != NULL) {
    str.append("with element ");
    str.append(lexeme);
    str.append("\n");
  }
  
  str.append(message);

  const char* c = str.c_str();
  uint32_t len = strlen(c);
  char* res = new char[len+1];
  strcpy(res, c);
  res[len] = '\0';
  
  return res;
}

const char* StaticTypeException::what() const throw() {

  std::string str = "";
  
  if(startLineNumber == endLineNumber) {
    str.append("StaticTypeException error on line ");
    str.append(std::to_string(startLineNumber));
  } else {
    str.append("StaticTypeException from line ");
    str.append(std::to_string(startLineNumber));
    str.append(" to line ");
    str.append(std::to_string(endLineNumber));
  }
  
  str.append("\n");
  str.append(context);
  
  //now append customized message if isCustomMessage
  if(isCustomMessage) {
    str.append(customMessage);

  } else {

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
  }
  
  const char* c = str.c_str();
  uint32_t len = strlen(c);
  char* res = new char[len+1];
  strcpy(res, c);
  res[len] = '\0';
  
  return res;
}


const char* StaticVariableScopeException::what() const throw() {

  std::string str = "StaticVariableScopeException on line ";
  str.append(std::to_string(lineNumber));
  str.append("\n");
  str.append(context);

  str.append("The variable ");
  str.append(variable);
  str.append(" ");

  if(isDeclaredError) {
    str.append("is already declared in this scope. It cannot be re-declared in the same scope.");
  } else {
    str.append("is not yet declared in this or enclosing scopes.\nIt has to be declared with a type before"
                " being accessed or assigned a value.");
  }

  const char* c = str.c_str();
  uint32_t len = strlen(c);
  char* res = new char[len+1];
  strcpy(res, c);
  res[len] = '\0';
  
  return res;
}

const char* StaticCastException::what() const throw() {

  std::string str = "";

  if(startLineNumber == endLineNumber) {
    str.append("StaticCastException on line ");
    str.append(std::to_string(startLineNumber));
  } else {
    str.append("StaticCastException from line ");
    str.append(std::to_string(startLineNumber));
    str.append(" to line ");
    str.append(std::to_string(endLineNumber));
  }

  str.append("\n");
  str.append(context);

  if(isExplicit) {
    str.append("Cannot cast at all from type ");
  } else {
    str.append("Cannot implicitly cast from type ");
  }
  
  str.append(toStringParseDataType(originalType));
  str.append(" to type ");
  str.append(toStringParseDataType(finalType));

  const char* c = str.c_str();
  uint32_t len = strlen(c);
  char* res = new char[len+1];
  strcpy(res, c);
  res[len] = '\0';
  
  return res;
}




