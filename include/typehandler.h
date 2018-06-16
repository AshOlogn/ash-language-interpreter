#ifndef TYPEHANDLER_H
#define TYPEHANDLER_H

#include "parsetoken.h"

//typecheck expression appropriately depending on type of AbstractExpressionNode
bool typecheckImplicitCastExpression(ParseDataType origType, ParseDataType finalType);

bool typecheckExplicitCastExpression(ParseDataType origType, ParseDataType finalType);

bool typecheckUnaryExpression(ParseOperatorType op, ParseDataType arg);

bool typecheckArithmeticExpression(ParseOperatorType op, ParseDataType l, ParseDataType r);

bool typecheckBitLogicalExpression(ParseOperatorType op, ParseDataType l, ParseDataType r);

bool typecheckComparisonExpression(ParseOperatorType op, ParseDataType l, ParseDataType r);

//typecheck expression appropriately depending on type of AbstractExpressionNode
ParseDataType getTypeUnaryExpression(ParseOperatorType op, ParseDataType arg);

ParseDataType getTypeArithmeticExpression(ParseOperatorType op, ParseDataType l, ParseDataType r);

ParseDataType getTypeBitLogicalExpression(ParseOperatorType op, ParseDataType l, ParseDataType r);

ParseDataType getTypeComparisonExpression(ParseOperatorType op, ParseDataType l, ParseDataType r);

#endif
