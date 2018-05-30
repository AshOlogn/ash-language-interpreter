#ifndef TOKEN_H
#define TOKEN_H

#include <cstdint>

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
  CLASS, FUN,

  TRUE, FALSE
};

//prints string representation of TokenType enum
const char* toStringTokenType(TokenType type);

//returns whether char represents a single-char Token
bool isSingleCharToken(char c);

//returns whether we have reached the end of the Token
bool isEndOfToken(char c);

//////////////Semantic information functions/////////////////////////

//returns whether Token is a type keyword
bool isTypeToken(TokenType tt);

//returns whether Token is an operator
bool isOperatorToken(TokenType tt);

//returns whether Token is a unary/binary operator
bool isUnaryOperatorToken(TokenType tt);
bool isBinaryOperatorToken(TokenType tt);

//returns whether Token is a comparator
bool isComparatorToken(TokenType tt);

//return whether a Token is assignment operator
bool isAssignmentOperatorToken(TokenType tt);

//////////////////////////////////////////////////////////////////////

//union that holds numerical value or pointer to string or object
union Data {
  uint64_t integer;
  double floatingPoint;
  void* allocated;
};

//struct that represents Token and relevant parsing information
struct Token {
  TokenType type;
  Data value;
  uint32_t line;
  const char* lexeme;
};

//full "constructor"
Token* makeToken(TokenType type, uint32_t line, char* lexeme, Data value);

//"constructor" for tokens without literal value
Token* makeToken(TokenType type, uint32_t line, char* lexeme);

#endif

