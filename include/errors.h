#ifndef ERRORS_H
#define ERRORS_H

#include <cstdint>
#include <string>
#include <exception>
#include "parsetoken.h"

//basic syntax errors in input source code
class LexerError : public std::exception {

  public:
    const char* context;
    const char* lexeme;
    const char* message;
    uint32_t lineNumber;
  
    LexerError(uint32_t line, const char* con, const char* lex, const char* mes) 
      : lineNumber{line}, context{con}, lexeme{lex}, message{mes} {}
    
    const char* what() const throw ();
};

class ParseSyntaxError : public std::exception {

  public:
    const char* context;
    const char* lexeme;
    const char* message;
    uint32_t startLineNumber;
    uint32_t endLineNumber;
    
    //single line, with lexeme specified
    ParseSyntaxError(uint32_t line, const char* con, const char* lex, const char* mes) 
      : startLineNumber{line}, endLineNumber{line}, context{con}, lexeme{lex}, message{mes} {}

    //single line, no lexeme specified
    ParseSyntaxError(uint32_t line, const char* con, const char* mes) 
      : startLineNumber{line}, endLineNumber{line}, context{con}, lexeme{NULL}, message{mes} {}

    //multiple lines, lexeme specified
    ParseSyntaxError(uint32_t startLine, uint32_t endLine, const char* con, const char* lex, const char* mes) 
      : startLineNumber{startLine}, endLineNumber{endLine}, context{con}, lexeme{lex}, message{mes} {}
		
    //multiple lines, no lexeme specified
    ParseSyntaxError(uint32_t startLine, uint32_t endLine, const char* con, const char* mes) 
      : startLineNumber{startLine}, endLineNumber{endLine}, context{con}, lexeme{NULL}, message{mes} {}
    
    const char* what() const throw ();
};

class StaticTypeError : public std::exception {

  private:
    const bool isBinary;
    const bool isCustomMessage;

  public:
    const char* context;
    const char* operation;
    const char* customMessage;
    const uint32_t startLineNumber;
    const uint32_t endLineNumber;
    ParseDataType arg1;
    ParseDataType arg2;
    
    StaticTypeError(uint32_t startLine, uint32_t endLine, const char* con, const char* op, ParseDataType a1) 
      : startLineNumber{startLine}, endLineNumber{endLine}, context{con}, customMessage{NULL}, isCustomMessage{false}, isBinary{false}, operation{op}, arg1{a1} {}
      
    StaticTypeError(uint32_t startLine, uint32_t endLine, const char* con, const char* op, ParseDataType a1, ParseDataType a2) 
      : startLineNumber{startLine}, endLineNumber{endLine}, context{con}, customMessage{NULL}, isCustomMessage{false}, isBinary{true}, operation{op}, arg1{a1}, arg2{a2} {}
    
    StaticTypeError(uint32_t startLine, uint32_t endLine, const char* con, const char* message) 
      : startLineNumber{startLine}, endLineNumber{endLine}, context{con}, customMessage{message}, isCustomMessage{true}, isBinary{false}, operation{NULL} {}

    const char* what() const throw ();
};

class StaticVariableScopeError : public std::exception {

  public:
    std::string variable;
    const char* context;
    const bool isDeclaredError;
    const uint32_t lineNumber;

    StaticVariableScopeError(uint32_t line, std::string var, const char* con, bool isDeclaredErr)
      : lineNumber(line), variable{var}, context{con}, isDeclaredError{isDeclaredErr} {}

    const char* what() const throw ();
};

class StaticCastError : public std::exception {

  public:
    const ParseDataType originalType;
    const ParseDataType finalType;
    const char* context;
		const char* message;
    const bool isExplicit;
    const uint32_t startLineNumber;
    const uint32_t endLineNumber;
    
    StaticCastError(uint32_t startLine, uint32_t endLine, const char* con, ParseDataType origType, ParseDataType finType, bool isExp)
      : startLineNumber{startLine}, endLineNumber{endLine}, context{con}, message{NULL}, originalType{origType}, finalType{finType}, isExplicit{isExp} {} 

		StaticCastError(uint32_t startLine, uint32_t endLine, const char* con, const char* msg)
      : startLineNumber{startLine}, endLineNumber{endLine}, context{con}, message{msg}, originalType{INVALID_T}, finalType{INVALID_T}, isExplicit{false} {}

    const char* what() const throw();
};



#endif

