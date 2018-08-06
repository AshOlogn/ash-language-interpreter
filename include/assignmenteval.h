#ifndef ASSIGNMENTEVAL_H
#define ASSIGNMENTEVAL_H

#include "token.h"
#include "parsenode.h"

ParseData evaluateAssignmentExpression(AssignmentExpressionNode* node);
ParseData evaluateArrayAssignmentExpression(ArrayAssignmentExpressionNode* node);

#endif