#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdint>
#include <algorithm>
#include "utils.h"
#include "token.h"

static const char* RESERVED_WORDS[] = {"for", "while", "do", "if", "elif", "else", "break",
                               "switch", "case", "class", "extends", "fun", "return", "new", "print", "println", "int8",
                              "int16", "int32", "int64", "uint8", "uint16",
                              "uint32", "uint64", "int", "long", "char", "double", "bool",
                              "string", "void", "true", "false"};

static const TokenType RESERVED_WORD_TOKENS[] = { FOR, WHILE, DO, IF, ELIF, ELSE, BREAK,
                                    SWITCH, CASE, CLASS, EXTENDS, FUN, RETURN, NEW, PRINT, PRINTLN,
																		INT8, INT16, INT32, INT64, UINT8, UINT16,
                                    UINT32, UINT64, INT32, INT64, CHAR, DOUBLE, BOOL,
                                    STRING, VOID, TRUE, FALSE};

static const uint8_t NUM_RESERVED_WORDS = 31;

//Token class constructors
//full "constructor"
Token makeToken(TokenType typ, uint32_t lin, const char* lex, Data val) {
  Token tokenPtr;
  tokenPtr.type = typ;
  tokenPtr.line = lin;
  tokenPtr.lexeme = copyString(lex);
  tokenPtr.value = val;
  return tokenPtr;
}

//"constructor" for tokens without literal value
Token makeToken(TokenType typ, uint32_t lin, const char* lex) {
  Token tokenPtr;
  tokenPtr.type = typ;
  tokenPtr.line = lin;
  tokenPtr.lexeme = copyString(lex);
  return tokenPtr;
}

//Functions acting on TokenType enum to get semantic and other information

//prints string representation of TokenType enum
const char* toStringTokenType(TokenType tokenType) {
  
  switch(tokenType) {
    case LEFT_PAREN: return "LEFT_PAREN";
    case RIGHT_PAREN: return "RIGHT_PAREN";
    case LEFT_BRACKET: return "LEFT_BRACKET";
    case RIGHT_BRACKET: return "RIGHT_BRACKET";
    case LEFT_BRACE: return "LEFT_BRACE";
    case RIGHT_BRACE: return "RIGHT_BRACE";
    case PERIOD: return "PERIOD";
    case ADD: return "ADD";
    case SUBTRACT: return "SUBTRACT";
    case NOT: return "NOT";
    case BIT_NOT: return "BIT_NOT";
    case EXPONENT: return "EXPONENT";
    case MULTIPLY: return "MULTIPLY";
    case DIVIDE: return "DIVIDE";
    case MOD: return "MOD";
    case BIT_LEFT: return "BIT_LEFT";
    case BIT_RIGHT: return "BIT_RIGHT";
    case GREATER: return "GREATER";
    case LESS: return "LESS";
    case GREATER_EQ: return "GREATER_EQ";
    case LESS_EQ: return "LESS_EQ";
    case EQ_EQ: return "EQ_EQ";
    case NOT_EQ: return "NOT_EQ";
    case BIT_AND: return "BIT_AND";
    case BIT_XOR: return "BIT_XOR";
    case BIT_OR: return "BIT_OR";
    case AND: return "AND";
    case XOR: return "XOR";
    case OR: return "OR";
    case QUESTION: return "QUESTION";
    case COLON: return "COLON";
    case EQ: return "EQ";
    case ADD_EQ: return "ADD_EQ";
    case SUBTRACT_EQ: return "SUBTRACT_EQ";
    case EXPONENT_EQ: return "EXPONENT_EQ";
    case MULTIPLY_EQ: return "MULTIPLY_EQ";
    case DIVIDE_EQ: return "DIVIDE_EQ";
    case MOD_EQ: return "MOD_EQ";
    case AND_EQ: return "AND_EQ";
    case XOR_EQ: return "XOR_EQ";
    case OR_EQ: return "OR_EQ";
    case BIT_LEFT_EQ: return "BIT_LEFT_EQ";
    case BIT_RIGHT_EQ: return "BIT_RIGHT_EQ";
    case COMMA: return "COMMA";
    case SEMICOLON: return "SEMICOLON";
    case INT8: return "INT8";
    case INT16: return "INT16";
    case INT32: return "INT32";
    case INT64: return "INT64";
    case UINT8: return "UINT8";
    case UINT16: return "UINT16";
    case UINT32: return "UINT32";
    case UINT64: return "UINT64";
    case CHAR: return "CHAR";
    case DOUBLE: return "DOUBLE";
    case BOOL: return "BOOL";
    case STRING: return "STRING";
    case VARIABLE: return "VARIABLE";
    case FOR: return "FOR";
    case WHILE: return "WHILE";
    case DO: return "DO";
    case IF: return "IF";
    case ELSE: return "ELSE";
    case BREAK: return "BREAK";
    case SWITCH: return "SWITCH";
    case CASE: return "CASE";
    case CLASS: return "CLASS";
    case FUN: return "FUN";
		case RETURN: return "RETURN";
		case NEW: return "NEW";
    case PRINT: return "PRINT";
    case PRINTLN: return "PRINTLN";
    case TRUE: return "TRUE";
    case FALSE: return "FALSE";
    case END: return "END";
    default: return "INVALID_TOKEN";
  }  
}


//returns a keyword token if input string matches one, VARIABLE otherwise
TokenType varOrKeywordTokenType(char* lexeme) {

  //determine if keyword, if not then identifier
  std::size_t len = strlen(lexeme);

  for(uint8_t i = 0; i < NUM_RESERVED_WORDS; i++) {
    const char* keyword = RESERVED_WORDS[i];
    if(strncmp(lexeme, keyword, std::max(len, strlen(keyword))) == 0) {
      return RESERVED_WORD_TOKENS[i];
    }
  }

  return VARIABLE;
}

//Returns whether character represents single-character Token
bool isSingleCharToken(char c) {  
  
  return c == '+' || c == '-' || c == '*' || c == '/' ||
         c == '%' || c == '&' || c == '|' || c == '^' ||
         c == '!' || c == '=' || c == '<' || c == '>' ||
         c == '?' || c == ':' || c == '.' || c == ',' || 
         c == ';' || c == '(' || c == ')' || c == '[' || 
         c == ']' || c == '{' || c == '}';
}

//Returns whether character represents end of a Token
bool isEndOfToken(char c) {
  return c == '\0' || isspace(c) || isSingleCharToken(c);
}

//////////////Semantic information functions/////////////////////////

//determines whether the token is a type keyword

bool isTypeTokenType(TokenType tt) { 
  return tt == INT8 || tt == INT16 || tt == INT32 || 
         tt == INT64 || tt == UINT8 || tt == UINT16 || 
         tt == UINT32 || tt == UINT64 || tt == CHAR ||
         tt == DOUBLE || tt == BOOL || tt == STRING;
}

//returns whether Token is an operator
bool isOperatorTokenType(TokenType tt) {

  return tt == LEFT_PAREN ||
         tt == RIGHT_PAREN || tt == LEFT_BRACKET || 
         tt == RIGHT_BRACKET || tt == LEFT_BRACE || 
         tt == RIGHT_BRACE || tt == PERIOD || tt == ADD || 
         tt == SUBTRACT || tt == NOT || tt == BIT_NOT ||

        tt == EXPONENT ||
        tt == MULTIPLY || tt == DIVIDE || tt == MOD ||
        tt == BIT_LEFT || tt == BIT_RIGHT ||
        
        tt == GREATER || tt == LESS || tt == GREATER_EQ || tt == LESS_EQ ||
        tt == EQ_EQ || tt == NOT_EQ ||

        tt == BIT_AND || tt == BIT_XOR || tt == BIT_OR ||  
        tt == AND || tt == XOR || tt == OR ||
        
        tt == EQ || tt == ADD_EQ || tt == SUBTRACT_EQ || tt == EXPONENT_EQ || 
        tt == MULTIPLY_EQ || tt == DIVIDE_EQ || tt == MOD_EQ || tt == AND_EQ || 
        tt == XOR_EQ || tt == OR_EQ || tt == BIT_LEFT_EQ || tt == BIT_RIGHT_EQ;
}

//returns whether Token is a unary/binary operator
bool isUnaryOperatorTokenType(TokenType tt) {

  return tt == ADD || tt == SUBTRACT || 
         tt == NOT || tt == BIT_NOT;
}

bool isBinaryOperatorTokenType(TokenType tt) {
  return tt == ADD || tt == SUBTRACT ||

        tt == EXPONENT ||
        tt == MULTIPLY || tt == DIVIDE || tt == MOD ||
        tt == BIT_LEFT || tt == BIT_RIGHT ||

        tt == BIT_AND || tt == BIT_XOR || tt == BIT_OR ||  
        tt == AND || tt == XOR || tt == OR;
}

//returns whether TokenType is +- or 
bool isSignNotTokenType(TokenType tt) {
  return tt == ADD || tt == SUBTRACT || tt == NOT || tt == BIT_NOT;
}

//returns whether TokenType is exponent
bool isExponentTokenType(TokenType tt) {
  return tt == EXPONENT;
}

//returns whether TokenType is multiply, divide, or mod
bool isMultiplyDivideModTokenType(TokenType tt) {
  return tt == MULTIPLY || tt == DIVIDE || tt == MOD;
}

//returns whether TokenType is add or subtract
bool isAddSubtractTokenType(TokenType tt) {
  return tt == ADD || tt == SUBTRACT;
}

//returns whether TokenType is bit shift
bool isBitShiftTokenType(TokenType tt) {
  return tt == BIT_LEFT || tt == BIT_RIGHT; 
}

//returns whether TokenType is an inequality comparator
bool isInequalityTokenType(TokenType tt) {
  return tt == GREATER || tt == LESS || 
         tt == GREATER_EQ || tt == LESS_EQ;
}

//returns whether TokenType is equality comparator
bool isEqualityTokenType(TokenType tt) {
  return tt == EQ_EQ || tt == NOT_EQ;
}

//return whether a TokenType is assignment operator
bool isAssignmentOperatorTokenType(TokenType tt) {
  return tt == EQ || tt == ADD_EQ || tt == SUBTRACT_EQ || tt == EXPONENT_EQ || 
         tt == MULTIPLY_EQ || tt == DIVIDE_EQ || tt == MOD_EQ || tt == AND_EQ || 
         tt == XOR_EQ || tt == OR_EQ || tt == BIT_LEFT_EQ || tt == BIT_RIGHT_EQ; 
}


