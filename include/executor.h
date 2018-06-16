#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <vector>
#include "parsetoken.h"
#include "parsenode.h"
#include "statementnode.h"


void executeExpressionStatement(ExpressionStatementNode* node);

void executePrintStatement(PrintStatementNode* node);

void executePrintLineStatement(PrintLineStatementNode* node);

void executeGroupedStatement(GroupedStatementNode* node);

void executeConditionalStatement(ConditionalStatementNode* node);


#endif