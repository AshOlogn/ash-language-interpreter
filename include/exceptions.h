#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <cstdint>
#include <exception>

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
    const char* lexeme;
    const char* lexeme2;
    const char* operation;
    const ParseDataType arg1;
    const ParseDataType arg2;
    const uint32_t lineNumber;
    
    StaticTypeException(uint32_t line, bool isBin, const char* con, const char* lex, const char* op, ParseDataType a1) 
      : lineNumber{line}, context{con}, lexeme{lex}, isBinary{false}, operation{op}, arg1{a1} {}
      
    StaticTypeException(uint32_t line, bool isBin, const char* con, const char* lex, const char* lex2, const char* op, ParseDataType a1, ParseDataType a2) 
      : lineNumber{line}, context{con}, lexeme{lex}, lexeme2{lex2}, isBinary{true}, operation{op}, arg1{a1}, arg2{a2} {}
    
    const char* what() const throw ();
};



#endif

