#include <cstdint>
#include <vector>
#include "token.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "parser.h"

using namespace std;

static uint32_t index;
static vector<Token*>* tokens;

//return current Token and advance
void consume() {
  return (Token*) &(tokens->at(index++));
}

//examine the current Token, don't advance position
Token* peek() {
  return (Token*) &(tokens->at(index));
}

//examine a Token at given index
Token* peek(uint32_t inputIndex) {
  return (Token*) &(tokens->at(inputIndex));
}


// variable, number, string, etc.
AbstractExpressionNode* evalLiteralGroup() {

  
}


// x++ x-- x() x->stuff x.stuff
AbstractExpressionNode* evalPostfixFunctionMembers() {
  return evalLiteralGroup();
}

// ++x --x + - ! ~
AbstractExpressionNode* evalPrefixNotCast() {

  AbstractExpressionNode* head = evalPrefixNotCast();

  while(isPrefixNotCastTokenType(peek()->type)) {
     
  }

  return head;
}

// **
AbstractExpressionNode* evalExponent() {

  AbstractExpressionNode* head = evalPrefixNotCast();

  while(isExponentTokenType(peek()->type)) {
     
  }
  
  return head;
}


// * / %
AbstractExpressionNode* evalMultiplyDivideMod() {

  AbstractExpressionNode* head = evalExponent();

  while(isMultiplyDivideModTokenType(peek()->type)) {
     
  }
  
  return head;
}


// + -
AbstractExpressionNode* evalAddSubtract() {

  AbstractExpressionNode* head = evalMultiplyDivideMod();

  while(isBitShiftTokenType(peek()->type)) {
     
  }
  
  return head;
}


// << >>
AbstractExpressionNode* evalBitShift() {

  AbstractExpressionNode* head = evalAddSubtract();

  while(isBitShiftTokenType(peek()->type)) {
     
  }
  
  return head;
}

// < <= > >=
AbstractExpressionNode* evalComparison() {

  AbstractExpressionNode* head = evalBitShift();

  while(isInequalityTokenType(peek()->type)) {
     
  }
  
  return head;
}


// == and !=
AbstractExpressionNode* evalEquality() {

  AbstractExpressionNode* head = evalComparison();

  while(isEqualityTokenType(peek()->type)) {
     
  }
 
  return head;
}


// &
AbstractExpressionNode* evalBitAnd() {
  return evalEquality();
  
}


// ^
AbstractExpressionNode* evalBitXor() {
  return evalBitAnd();
  
}


// |
AbstractExpressionNode* evalBitOr() {

  return evalBitXor();
}


// &&
AbstractExpressionNode* evalLogicAnd() {
  return evalBitOr();
  
}


// ^^
AbstractExpressionNode* evalLogicXor() {
  return evalLogicAnd();
  
}


// ||
AbstractExpressionNode* evalLogicOr() {
  return evalLogicXor();
  
}


// ? :
AbstractExpressionNode* evalTernary() {

  return evalLogicOr();
}


//  = += -= *= **= /= &= ^= |= <<= >>=
AbstractExpressionNode* evalAssignment() {
  return evalTernary();
  
}



//generate Abstract Syntax Tree from list of tokens
AbstractExpressionNode* parse(vector<Token>* tokenRef) {

  //set static variables to correct initial values
  index = 0;
  tokens = tokenRef;

  return evalAssignment();
}


