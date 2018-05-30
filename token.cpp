#include <cstdint>
#include <iostream>
#include "token.h"

//Token class constructors
//full "constructor"
Token* makeToken(TokenType typ, uint32_t lin, char* lex, Data val) {
  Token* tokenPtr = (Token*) malloc(sizeof(Token));
  tokenPtr->type = typ;
  tokenPtr->line = lin;
  tokenPtr->lexeme = lex;
  tokenPtr->value = val;
  return tokenPtr;
}

//"constructor" for tokens without literal value
Token* makeToken(TokenType typ, uint32_t lin, char* lex) {
  Token* tokenPtr = (Token*) malloc(sizeof(Token));
  tokenPtr->type = typ;
  tokenPtr->line = lin;
  tokenPtr->lexeme = lex;
  return tokenPtr;
}

//Functions acting on TokenType enum to get semantic and other information

//prints string representation of TokenType enum
const char* toStringTokenType(TokenType tokenType) {
  
  switch(tokenType) {
    case INC: return "INC";
    case DEC: return "DEC";
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
    case TRUE: return "TRUE";
    case FALSE: return "FALSE";
    default: return "INVALID_TOKEN";
  }  
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
bool isTypeToken(TokenType tt) { 
  return tt == INT8 || tt == INT16 || tt == INT32 || 
         tt == INT64 || tt == UINT8 || tt == UINT16 || 
         tt == UINT32 || tt == UINT64 || tt == CHAR ||
         tt == DOUBLE || tt == BOOL || tt == STRING ||
         tt == VARIABLE;
}

//returns whether Token is an operator
bool isOperatorToken(TokenType tt) {
  return tt == INC || tt == DEC || tt == LEFT_PAREN ||
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
        tt == AND || tt == XOR || tt == OR,
        
        tt == EQ || tt == ADD_EQ || tt == SUBTRACT_EQ || tt == EXPONENT_EQ || 
        tt == MULTIPLY_EQ || tt == DIVIDE_EQ || tt == MOD_EQ || tt == AND_EQ || 
        tt == XOR_EQ || tt == OR_EQ || tt == BIT_LEFT_EQ || tt == BIT_RIGHT_EQ;
}

//returns whether Token is a unary/binary operator
bool isUnaryOperatorToken(TokenType tt) {
  return tt == INC || tt == DEC || 
         tt == ADD || tt == SUBTRACT || 
         tt == NOT || tt == BIT_NOT;
}

bool isBinaryOperatorToken(TokenType tt) {
  return tt == ADD || tt == SUBTRACT ||

        tt == EXPONENT ||
        tt == MULTIPLY || tt == DIVIDE || tt == MOD ||
        tt == BIT_LEFT || tt == BIT_RIGHT ||

        tt == BIT_AND || tt == BIT_XOR || tt == BIT_OR ||  
        tt == AND || tt == XOR || tt == OR;
}

//returns whether Token is a comparator
bool isComparatorToken(TokenType tt) {
  return tt == GREATER || tt == LESS || 
         tt == GREATER_EQ || tt == LESS_EQ ||
         tt == EQ_EQ || tt == NOT_EQ;
}

//return whether a Token is assignment operator
bool isAssignmentOperatorToken(TokenType tt) {
  return tt == EQ || tt == ADD_EQ || tt == SUBTRACT_EQ || tt == EXPONENT_EQ || 
         tt == MULTIPLY_EQ || tt == DIVIDE_EQ || tt == MOD_EQ || tt == AND_EQ || 
         tt == XOR_EQ || tt == OR_EQ || tt == BIT_LEFT_EQ || tt == BIT_RIGHT_EQ; 
}



