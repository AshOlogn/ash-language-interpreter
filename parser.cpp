#include <cstdint>
#include <vector>
#include "token.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "parser.h"

using namespace std;

static uint32_t index;
static vector<Token*>* tokens;

void consume() {
  index++;
}


// variable, number, string, etc.
AbstractExpressionNode* evalLiteralGroup() {

  
}


// x++ x-- x() x->stuff x.stuff
AbstractExpressionNode* evalPostfixFunctionMembers() {

  
}

// ++x --x + - ! ~
AbstractExpressionNode* evalPrefixNotCast() {

  
}

// **
AbstractExpressionNode* evalExponent() {

  
}


// * / %
AbstractExpressionNode* evalMultiplyDivideMod() {

  
}


// + -
AbstractExpressionNode* evalAddSubtract() {

  
}


// << >>
AbstractExpressionNode* evalBitShift() {

  
}

// < <= > >=
AbstractExpressionNode* evalComparison() {

  
}


// == and !=
AbstractExpressionNode* evalEquality() {

  
}


// &
AbstractExpressionNode* evalBitAnd() {

  
}


// ^
AbstractExpressionNode* evalBitXor() {

  
}


// |
AbstractExpressionNode* evalBitOr() {

  
}


// &&
AbstractExpressionNode* evalLogicAnd() {

  
}


// ^^
AbstractExpressionNode* evalLogicXor() {

  
}


// ||
AbstractExpressionNode* evalLogicOr() {

  
}


// ? :
AbstractExpressionNode* evalTernary() {

  
}


//  = += -= *= **= /= &= ^= |= <<= >>=
AbstractExpressionNode* evalAssignment() {

  
}



//generate Abstract Syntax Tree from list of tokens
AbstractExpressionNode* parse(vector<Token*>* tokenRef) {

  //set static variables to correct initial values
  index = 0;
  tokens = tokenRef;

  return evalAssignment();
}
