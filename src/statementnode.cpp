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

//represents while loop
WhileStatementNode::WhileStatementNode(AbstractExpressionNode* cond, AbstractStatementNode* bod, SymbolTable* table) {
  condition = cond;
  body = bod;
  symbolTable = table;
}

void WhileStatementNode::execute() {
  
  //execute the body while the condition evaluates to true
  ParseData d = condition->evaluate();
  while(d.type == BOOL_T && d.value.integer) {
    
    body->execute();
    
    //get updated expression truth value
    d = condition->evaluate();
  }
}

//represents for loop
ForStatementNode::ForStatementNode(AbstractStatementNode* init, AbstractStatementNode* upd, AbstractStatementNode* bod, AbstractExpressionNode* cond, SymbolTable* table) {
  initialization = init;
  update = upd;
  body = bod;
  condition = cond;
  symbolTable = table;
}

void ForStatementNode::execute() {

  symbolTable->enterNewScope();

  //first run initialization statement
  initialization->execute();
  
  //now execute the body while the terminating condition is true
  ParseData d = condition->evaluate();
  while(d.type == BOOL_T && d.value.integer) {
    body->execute();
    
    //update statement
    update->execute();
    d = condition->evaluate();
  }
  
  symbolTable->leaveScope();
}

//represents declaration (and maybe assignment) of a new variable
NewAssignmentStatementNode::NewAssignmentStatementNode(std::string var, ParseDataType typ, AbstractExpressionNode* val, SymbolTable* table) {
  
	variable = var;
	type = typ;
	value = val;
	symbolTable = table;
  
  //add variable to table with correctly-typed dummy value    
  ParseData d;
  d.type = type;
  symbolTable->declare(variable, d);
}

NewAssignmentStatementNode::NewAssignmentStatementNode(std::string var, ParseDataType typ, SymbolTable* table) {
	variable = var;
	type = typ;
  value = NULL;
	symbolTable = table;
  
  //add variable to table with correctly-typed dummy value    
  ParseData d;
  d.type = type;
  symbolTable->declare(variable, d);
}

void NewAssignmentStatementNode::execute() {
	executeNewAssignmentStatement(this);
}

//represents existing variable assignment
AssignmentStatementNode::AssignmentStatementNode(std::string var, AbstractExpressionNode* val, SymbolTable* table) {
  variable = var;
  value = val;
  symbolTable = table;
}

void AssignmentStatementNode::execute() {
	executeAssignmentStatement(this);
}

//represents return statement in a function
ReturnStatementNode::ReturnStatementNode(AbstractExpressionNode* exp, bool* retFlag, ParseData* retVal) {
	expression = exp;
	returnFlag = retFlag;
	returnValue = retVal;
}

void ReturnStatementNode::execute() {
	executeReturnStatement(this);
}

//represents function declaration (and maybe definition)
FunctionStatementNode::FunctionStatementNode(std::string fName, Function* f, SymbolTable* table) {
	functionName = fName;
	function = f;
	symbolTable = table;

	ParseData d;
	d.type = FUN_T;
	d.value.allocated = f;
	//table->declare(fName, d);
}

FunctionStatementNode::FunctionStatementNode(std::string fName, SymbolTable* table) {
	functionName = fName;
	function = NULL;
	symbolTable = table;

	ParseData d;
	d.type = FUN_T;
	//table->declare(fName, d);
}

void FunctionStatementNode::execute() {
	executeFunctionStatement(this);
}


