#include <iostream>
#include <vector>
#include "parsetoken.h"
#include "parsenode.h"
#include "statementnode.h"
#include "symboltable.h"
#include "executor.h"

//represents a single-expression statement
ExpressionStatementNode::ExpressionStatementNode(AbstractExpressionNode* exp, SymbolTable* table) {
  expression = exp;
  symbolTable = table;
}

void ExpressionStatementNode::execute() {
  executeExpressionStatement(this);
}

//represents print statement (no new line character)
PrintStatementNode::PrintStatementNode(AbstractExpressionNode* exp, SymbolTable* table) {
  expression = exp;
  symbolTable = table;
}

void PrintStatementNode::execute() {
  executePrintStatement(this);
}

//represents print statement (yes new line character)
PrintLineStatementNode::PrintLineStatementNode(AbstractExpressionNode* exp, SymbolTable* table) {
  expression = exp;
  symbolTable = table;
}

void PrintLineStatementNode::execute() {
  executePrintLineStatement(this);
}

//represents a group of statements in braces
GroupedStatementNode::GroupedStatementNode(std::vector<AbstractStatementNode*>* s, SymbolTable* table) {
  statements = s;
  symbolTable = table;
}

void GroupedStatementNode::execute() {
  executeGroupedStatement(this);
}


//represents if-elif-else structure
ConditionalStatementNode::ConditionalStatementNode(std::vector<AbstractExpressionNode*>* cond, std::vector<AbstractStatementNode*>* stat, SymbolTable* table) {
  conditions = cond;
  statements = stat;
  symbolTable = table;
}

void ConditionalStatementNode::execute() {
  executeConditionalStatement(this);
}

//represents variable declaration and assignment
AssignmentStatementNode::AssignmentStatementNode(char* var, ParseData val, SymbolTable* table) {
  variable = var;
  value = val;
  symbolTable = table;
}

void AssignmentStatementNode::execute() {
  
}







