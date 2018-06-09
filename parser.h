#include <vector>
#include <cstdint>
#include "token.h"
#include "parsetoken.h"
#include "parsenode.h"

/////////////////////////////////
//////     Return Tree     //////
/////////////////////////////////

AbstractExpressionNode* parse(std::vector<Token>* tokens);

/////////////////////////////////
//////    Access Tokens     /////
/////////////////////////////////

//consume a Token in the list (increment index)
Token* consume();

//examine the current Token, don't advance position
Token* peek();

//examine a Token at given index
Token* peek(uint32_t inputIndex);


/////////////////////////////////
//////    Subroutines     ///////
/////////////////////////////////

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

AbstractExpressionNode* evalExpression();

