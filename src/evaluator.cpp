#include <iostream>
#include <cstdint>
#include <cmath>
#include <cstring>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"

//recursively evaluate AST, start at root
ParseData evaluate(AbstractExpressionNode* node) {
  return node->evaluate();
}



