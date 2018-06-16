#ifndef CASTEVAL_H
#define CASTEVAL_H


#include "parsetoken.h"
#include "parsenode.h"

ParseData castHelper(ParseData orig, ParseDataType finalType);
ParseData evaluateCastExpression(CastNode* node);

#endif