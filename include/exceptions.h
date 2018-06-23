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


#endif