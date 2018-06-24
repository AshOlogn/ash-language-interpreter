#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <cstdint>
#include <exception>
#include "parsetoken.h"

//basic syntax errors in input source code
class LexerException : public std::exception {

  public:
    const char* context;
    const char* lexeme;
    const char* message;
    uint32_t lineNumber;
  
    LexerException(uint32_t line, const char* con, const char* lex, const char* mes) 
      : lineNumber{line}, context{con}, lexeme{lex}, message{mes} {}
    
    const char* what() const throw ();
};

class ParseSyntaxException : public std::exception {

  public:
    const char* context;
    const char* lexeme;
    const char* message;
    uint32_t lineNumber;
    
    ParseSyntaxException(uint32_t line, const char* con, const char* lex, const char* mes) 
      : lineNumber{line}, context{con}, lexeme{lex}, message{mes} {}
    
    const char* what() const throw ();
};

class StaticTypeException : public std::exception {

  private:
    const bool isBinary;
  
  public:
    const char* context;
    const char* operation;
    const uint32_t startLineNumber;
    const uint32_t endLineNumber;
    ParseDataType arg1;
    ParseDataType arg2;
    
    StaticTypeException(uint32_t startLine, uint32_t endLine, const char* con, const char* op, ParseDataType a1) 
      : startLineNumber{startLine}, endLineNumber{endLine}, context{con}, isBinary{false}, operation{op}, arg1{a1} {}
      
    StaticTypeException(uint32_t startLine, uint32_t endLine, const char* con, const char* op, ParseDataType a1, ParseDataType a2) 
      : startLineNumber{startLine}, endLineNumber{endLine}, context{con}, isBinary{true}, operation{op}, arg1{a1}, arg2{a2} {}
    
    const char* what() const throw ();
};



#endif

