#include <iostream>
#include <cstdint>
#include <cmath>
#include <cstring>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "array.h"

//just return the value represented by node's ParseData
ParseData evaluateLiteralExpression(LiteralNode* node) {
	return node->data;
}