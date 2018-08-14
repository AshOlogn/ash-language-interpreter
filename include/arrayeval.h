#ifndef ARRAYEVAL_H
#define ARRAYEVAL_H

#include "parsetoken.h"
#include "parsenode.h"

//evaluate expression that produces an array
ParseData evaluateArrayExpression(ArrayNode* node);

#endif