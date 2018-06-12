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
  CLASS, FUN, PRINT, PRINTLN,

  TRUE, FALSE,

  //end of program, invalid Token
  END, INVALID
};

//prints string representation of TokenType enum
const char* toStringTokenType(TokenType type);

//returns whether char represents a single-char Token
bool isSingleCharToken(char c);

//returns whether we have reached the end of the Token
bool isEndOfToken(char c);

//returns a keyword token if input string matches one, INVALID otherwise
TokenType varOrKeywordTokenType(char* input);

//////////////Semantic information functions/////////////////////////

//returns whether TokenType is a type keyword
bool isTypeTokenType(TokenType tt);

//returns whether TokenType is an operator
bool isOperatorTokenType(TokenType tt);

//returns whether TokenType is a unary/binary operator
bool isUnaryOperatorTokenType(TokenType tt);
bool isBinaryOperatorTokenType(TokenType tt);

//returns whether TokenType is sign or bit/logical NOT
bool isSignNotTokenType(TokenType tt);

//returns whether TokenType is exponent
bool isExponentTokenType(TokenType tt);

//returns whether TokenType is multiply, divide, or mod
bool isMultiplyDivideModTokenType(TokenType tt);

//returns whether TokenType is add or subtract
bool isAddSubtractTokenType(TokenType tt);

//returns whether TokenType is bit shift
bool isBitShiftTokenType(TokenType tt);

//returns whether TokenType is an inequality comparator
bool isInequalityTokenType(TokenType tt);

//returns whether TokenType is an equality comparator
bool isEqualityTokenType(TokenType tt);

//return whether a Token is assignment operator
bool isAssignmentOperatorTokenType(TokenType tt);

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
Token makeToken(TokenType type, uint32_t line, char* lexeme, Data value);

//"constructor" for tokens without literal value
Token makeToken(TokenType type, uint32_t line, char* lexeme);

#endif

