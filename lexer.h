#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <iostream>

//represents the different types of tokens generated from raw text input
enum TokenType {
  
  //Operators and Symbols (in order of precedence)
  INC, DEC, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET,
  LEFT_BRACE, RIGHT_BRACE, PERIOD, ADD, SUBTRACT, NOT, BIT_NOT,

  EXPONENT, 
  MULTIPLY, DIVIDE, MOD,
  BIT_LEFT, BIT_RIGHT,
  
  GREATER, LESS, GREATER_EQ, LESS_EQ,
  EQ_EQ, NOT_EQ,

  BIT_AND,
  BIT_XOR,
  BIT_OR,  

  AND,
  XOR,
  OR,

  QUESTION, COLON,
  EQ, ADD_EQ, SUBTRACT_EQ, EXPONENT_EQ, MULTIPLY_EQ, DIVIDE_EQ, MOD_EQ, 
  AND_EQ, XOR_EQ, OR_EQ, BIT_LEFT_EQ, BIT_RIGHT_EQ,
  
  COMMA, SEMICOLON,

  //Types
  INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, 
  CHAR, DOUBLE, BOOL, STRING, VARIABLE,

  //Reserved Words
  FOR, WHILE, DO, IF, ELSE, BREAK, SWITCH, CASE,
  CLASS, FUN
};

//union that holds numerical value or pointer to string or object
union TokenValue {
  uint64_t integer;
  double floatingPoint;
  void* allocated;
};

//class that represents Token and relevant parsing information
class Token {
  public:
    TokenType type;
    uint32_t line;
    char* lexeme;
    TokenValue value;

    Token(TokenType type, uint32_t line, char* lexeme, TokenValue value);
    Token(TokenType type, uint32_t line, char* lexeme);
};


//returns whether Token is a type keyword
bool isTypeToken(TokenType token_type);

//returns whether char represents a single-char Token
bool isSingleCharToken(char c);

//returns whether we have reached the end of the Token
bool isEndOfToken(char c);

//lexer error reporting
void lexError(const char* message);

//convert literals to actual numerical values
uint64_t stringToInt(char* str);
double stringToDouble(char* str);

//generates list of Tokens from Ash source code
std::vector<Token> lex(char* code);

#endif
