#include <iostream>
#include <cstdint>
#include <vector>
#include "token.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "parser.h"

using namespace std;

static uint32_t index;
static vector<Token>* tokens;

/////////////////////////////////
//////    Access Tokens     /////
/////////////////////////////////

//return current Token and advance
Token* consume() {
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


/////////////////////////////////
//////    Subroutines     ///////
/////////////////////////////////

// variable, number, string, etc.
AbstractExpressionNode* evalLiteralGroup() {

  AbstractExpressionNode* next;
 
  if(peek()->type == LEFT_PAREN) {

    consume(); //consume left parenthesis
    next = evalAddSubtract();

    if(peek()->type != RIGHT_PAREN) {
      //error
      cout << "ERROR!!!!" << endl;
    } else {
      consume(); //consume right parenthesis
    }

    return new GroupedExpressionNode(next);

  } else {

    ParseData p = {UINT64_T, (consume())->value};
    return new LiteralNode(p);
  }
}

// **
AbstractExpressionNode* evalExponent() {

  AbstractExpressionNode* head = evalLiteralGroup();
  AbstractExpressionNode* next;

  while(peek()->type == EXPONENT) {
    consume(); 
    next = evalLiteralGroup();
    head = new ArithmeticOperatorNode(EXPONENT_OP, head, next);
  }
  
  return head;
}


// * / %
AbstractExpressionNode* evalMultiplyDivideMod() {

  AbstractExpressionNode* head = evalExponent();
  AbstractExpressionNode* next;

  while(isMultiplyDivideModTokenType(peek()->type)) {
    ParseOperatorType op = binaryTokenConversion(consume()->type);
    next = evalExponent();
    head = new ArithmeticOperatorNode(op, head, next);  
  }
  return head;
}


// + -
AbstractExpressionNode* evalAddSubtract() {

  AbstractExpressionNode* head = evalMultiplyDivideMod();
  AbstractExpressionNode* next;

  while(isAddSubtractTokenType(peek()->type)) {
    ParseOperatorType op = binaryTokenConversion(consume()->type);
    next = evalMultiplyDivideMod();
    head = new ArithmeticOperatorNode(op, head, next);   
  }
  
  return head;
}

// << >>
AbstractExpressionNode* evalBitShift() {

  AbstractExpressionNode* head = evalAddSubtract();
  AbstractExpressionNode* next;

  while(isBitShiftTokenType(peek()->type)) {
    ParseOperatorType op = binaryTokenConversion(consume()->type);
    next = evalAddSubtract();
    head = new BitLogicalOperatorNode(op, head, next);  
  }
  
  return head;
}


// < <= > >=
AbstractExpressionNode* evalComparison() {

  AbstractExpressionNode* head = evalBitShift();
  AbstractExpressionNode* next;

  while(isInequalityTokenType(peek()->type)) {
    ParseOperatorType op = binaryTokenConversion(consume()->type);
    next = evalBitShift();
    head = new ComparisonOperatorNode(op, head, next);
  }
  
  return head;
}

// == and !=
AbstractExpressionNode* evalEquality() {

  AbstractExpressionNode* head = evalComparison();
  AbstractExpressionNode* next;

  while(isEqualityTokenType(peek()->type)) {
    ParseOperatorType op = binaryTokenConversion(consume()->type);
    next = evalComparison();
    head = new ComparisonOperatorNode(op, head, next);
  }
 
  return head;
}

// &
AbstractExpressionNode* evalBitAnd() {

  AbstractExpressionNode* head = evalEquality();
  AbstractExpressionNode* next;

  while(peek()->type == BIT_AND) {
    consume();
    next = evalEquality();
    head = new BitLogicalOperatorNode(BIT_AND_OP, head, next);
  }
 
  return head;
}


// ^
AbstractExpressionNode* evalBitXor() {

  AbstractExpressionNode* head = evalBitAnd();
  AbstractExpressionNode* next;

  while(peek()->type == BIT_XOR) {
    consume();
    next = evalBitAnd();
    head = new BitLogicalOperatorNode(BIT_XOR_OP, head, next);
  }
 
  return head;
}

// |
AbstractExpressionNode* evalBitOr() {

  AbstractExpressionNode* head = evalBitXor();
  AbstractExpressionNode* next;

  while(peek()->type == BIT_OR) {
    consume();
    next = evalBitXor();
    head = new BitLogicalOperatorNode(BIT_OR_OP, head, next);
  }
 
  return head;
}


// &&
AbstractExpressionNode* evalLogicAnd() {

  AbstractExpressionNode* head = evalBitOr();
  AbstractExpressionNode* next;

  while(peek()->type == AND) {
    consume();
    next = evalBitOr();
    head = new BitLogicalOperatorNode(AND_OP, head, next);
  }
 
  return head;
}


// ^^
AbstractExpressionNode* evalLogicXor() {

  AbstractExpressionNode* head = evalLogicAnd();
  AbstractExpressionNode* next;

  while(peek()->type == XOR) {
    consume();
    next = evalLogicAnd();
    head = new BitLogicalOperatorNode(XOR_OP, head, next);
  }
 
  return head;
}

// ||
AbstractExpressionNode* evalLogicOr() {

  AbstractExpressionNode* head = evalLogicXor();
  AbstractExpressionNode* next;

  while(peek()->type == OR) {
    consume();
    next = evalLogicXor();
    head = new BitLogicalOperatorNode(OR_OP, head, next);
  }
 
  return head;
}


//  = += -= *= **= /= &= ^= |= <<= >>=
AbstractExpressionNode* evalAssignment() {
  return evalLogicOr();
  
}

//generate Abstract Syntax Tree from list of tokens
AbstractExpressionNode* parse(std::vector<Token>* tokenRef) {

  //set static variables to correct initial values
  index = 0;
  tokens = tokenRef;

  return evalAssignment();
}


