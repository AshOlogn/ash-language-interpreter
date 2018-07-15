#include <iostream>
#include <cstring>
#include <string>
#include <exception>
#include "utils.h"
#include "parsetoken.h"
#include "errors.h"

const char* LexerError::what() const throw() {
  
  std::string str  = "LexerError on line ";
  str.append(std::to_string(lineNumber));
  str.append(":\n\t");
  str.append(context);
  str.append("with token ");
  str.append(lexeme);
  str.append("\n");
  str.append(message);
  
  return copyString(str.c_str());
}

const char* ParseSyntaxError::what() const throw() {
  
  std::string str = "";
  
  if(startLineNumber == endLineNumber) {
    str.append("ParseSyntaxError error on line ");
    str.append(std::to_string(startLineNumber));
  } else {
    str.append("ParseSyntaxError error from line ");
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

  return copyString(str.c_str());
}

const char* StaticTypeError::what() const throw() {

  std::string str = "";
  
  if(startLineNumber == endLineNumber) {
    str.append("StaticTypeError error on line ");
    str.append(std::to_string(startLineNumber));
  } else {
    str.append("StaticTypeError from line ");
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
  
  return copyString(str.c_str());
}


const char* StaticVariableScopeError::what() const throw() {

  std::string str = "StaticVariableScopeError on line ";
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

  return copyString(str.c_str());
}

const char* StaticCastError::what() const throw() {

  std::string str = "";

  if(startLineNumber == endLineNumber) {
    str.append("StaticCastError on line ");
    str.append(std::to_string(startLineNumber));
  } else {
    str.append("StaticCastError from line ");
    str.append(std::to_string(startLineNumber));
    str.append(" to line ");
    str.append(std::to_string(endLineNumber));
  }

  str.append("\n");
  str.append(context);

	if(message != NULL) {
		str.append(message);
		return copyString(str.c_str());
	}

  if(isExplicit) {
    str.append("Cannot cast at all from type ");
  } else {
    str.append("Cannot implicitly cast from type ");
  }
  
  str.append(toStringParseDataType(originalType));
  str.append(" to type ");
  str.append(toStringParseDataType(finalType));

  return copyString(str.c_str());
}




