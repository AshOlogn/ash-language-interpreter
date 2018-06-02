#include "token.h"
#include "parsetoken.h"

// variable, number, string, etc.
ParseData* resLiteralGroup(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// x++ x-- x() x->stuff x.stuff
ParseData* resPostfixFunctionMembers(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// ++x --x + - ! ~
ParseData* resPrefixNotCast(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// **
ParseData* resExponent(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// * / %
ParseData* resMultiplyDivideMod(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// + -
ParseData* resAddSubtract(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// << >>
ParseData* resBitShift(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// < <= > >=
ParseData* resComparison(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// == and !=
ParseData* resEquality(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// &
ParseData* resBitAnd(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// ^
ParseData* resBitXor(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// |
ParseData* resBitOr(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// &&
ParseData* resLogicAnd(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// ^^
ParseData* resLogicXor(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// ||
ParseData* resLogicOr(ParseOperatorType op, ParseData left, ParseData right) {
  
}

// ? :
ParseData* resTernary(ParseOperatorType op, ParseData left, ParseData right) {
  
}

//  = += -= *= **= /= &= ^= |= <<= >>=
ParseData* resAssignment(ParseOperatorType op, ParseData left, ParseData right) {
  
}




