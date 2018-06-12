#include <cstdint>
#include <cmath>
#include <cstring>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"

//just evaluate subtree representing parenthesized expression
ParseData evaluateGroupedExpression(AbstractExpressionNode* node) {
  return ((GroupedExpressionNode*) node)->closedExpression->evaluate();
}