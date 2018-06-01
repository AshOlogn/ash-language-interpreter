#include <vector>
#include "token.h"
#include "parsetoken.h"
#include "parsenode.h"

//return Abstract Syntax Tree 
AbstractExpressionNode* parse(vector<Token*>* tokens);

/////////////////////////////////
//////    Subroutines     ///////
/////////////////////////////////

//consume a token in the list (increment index)
void consume();

// variable, number, string, etc.
AbstractExpressionNode* evalLiteralGroup();

// x++ x-- x() x->stuff x.stuff
AbstractExpressionNode* evalPostfixFunctionMembers();

// ++x --x + - ! ~
AbstractExpressionNode* evalPrefixNotCast();

// **
AbstractExpressionNode* evalExponent();

// * / %
AbstractExpressionNode* evalMultiplyDivideMod();

// + -
AbstractExpressionNode* evalAddSubtract();

// << >>
AbstractExpressionNode* evalBitShift();

// < <= > >=
AbstractExpressionNode* evalComparison();

// == and !=
AbstractExpressionNode* evalEquality();

// &
AbstractExpressionNode* evalBitAnd();

// ^
AbstractExpressionNode* evalBitXor();

// |
AbstractExpressionNode* evalBitOr();

// &&
AbstractExpressionNode* evalLogicAnd();

// ^^
AbstractExpressionNode* evalLogicXor();

// ||
AbstractExpressionNode* evalLogicOr();

// ? :
AbstractExpressionNode* evalTernary();

//  = += -= *= **= /= &= ^= |= <<= >>=
AbstractExpressionNode* evalAssignment();


