#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "token.h"
#include "parsenode.h"

//class that specifies which version of evaluate() 
//to use for each expression class

class ExpressionEvaluator {
  public:
    ParseData evaluateExpression(AbstractExpressionNode* node);
};


#endif
