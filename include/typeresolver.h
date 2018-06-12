#ifndef TYPERESOLVER_H
#define TYPERESOLVER_H

#include "token.h"
#include "parsetoken.h"

// x++ x-- x() x->stuff x.stuff
ParseData* resPostfixFunctionMembers();

// ++x --x + - ! ~
ParseData* resPrefixNotCast(ParseOperatorType op, ParseData data);

// **
ParseData* resExponent(ParseOperatorType op, ParseData left, ParseData right);

// * / %
ParseData* resMultiplyDivideMod(ParseOperatorType op, ParseData left, ParseData right);

// + -
ParseData* resAddSubtract(ParseOperatorType op, ParseData left, ParseData right);

// << >>
ParseData* resBitShift(ParseOperatorType op, ParseData left, ParseData right);

// < <= > >=
ParseData* resComparison(ParseOperatorType op, ParseData left, ParseData right);

// == and !=
ParseData* resEquality(ParseOperatorType op, ParseData left, ParseData right);

// &
ParseData* resBitAnd(ParseOperatorType op, ParseData left, ParseData right);

// ^
ParseData* resBitXor(ParseOperatorType op, ParseData left, ParseData right);

// |
ParseData* resBitOr(ParseOperatorType op, ParseData left, ParseData right);

// &&
ParseData* resLogicAnd(ParseOperatorType op, ParseData left, ParseData right);

// ^^
ParseData* resLogicXor(ParseOperatorType op, ParseData left, ParseData right);

// ||
ParseData* resLogicOr(ParseOperatorType op, ParseData left, ParseData right);

// ? :
ParseData* resTernary(ParseOperatorType op, ParseData left, ParseData right);

//  = += -= *= **= /= &= ^= |= <<= >>=
ParseData* resAssignment(ParseOperatorType op, ParseData left, ParseData right);

#endif


