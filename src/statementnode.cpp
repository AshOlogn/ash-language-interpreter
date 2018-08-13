#include <iostream>
#include <vector>
#include "parsetoken.h"
#include "parsenode.h"
#include "statementnode.h"
#include "symboltable.h"
#include "executor.h"
#include "array.h"

//represents a single-expression statement
ExpressionStatementNode::ExpressionStatementNode(AbstractExpressionNode* exp, SymbolTable* symbolTable) {
  expression = exp;
  this->symbolTable = symbolTable;
	startLine = exp->startLine;
	endLine = exp->endLine;
}

void ExpressionStatementNode::execute() {
  executeExpressionStatement(this);
}

//represents print statement (no new line character)
PrintStatementNode::PrintStatementNode(AbstractExpressionNode* exp, SymbolTable* symbolTable, uint32_t startLine) {
  expression = exp;
  this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = exp->endLine;
}

void PrintStatementNode::execute() {
  executePrintStatement(this);
}

//represents print statement (yes new line character)
PrintLineStatementNode::PrintLineStatementNode(AbstractExpressionNode* exp, SymbolTable* symbolTable, uint32_t startLine) {
  expression = exp;
  this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = exp->endLine;
}

void PrintLineStatementNode::execute() {
  executePrintLineStatement(this);
}


//represents a group of statements in braces
GroupedStatementNode::GroupedStatementNode(std::vector<AbstractStatementNode*>* s, SymbolTable* symbolTable, uint32_t startLine, uint32_t endLine) {
  statements = s;
  this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = endLine;
}

void GroupedStatementNode::execute() {
  executeGroupedStatement(this);
}

//represents if-elif-else structure
ConditionalStatementNode::ConditionalStatementNode(std::vector<AbstractExpressionNode*>* cond, std::vector<AbstractStatementNode*>* stat, SymbolTable* symbolTable, uint32_t startLine, uint32_t endLine) {
  conditions = cond;
  statements = stat;
  this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = endLine;
}

void ConditionalStatementNode::execute() {
  executeConditionalStatement(this);
}

//represents while loop
WhileStatementNode::WhileStatementNode(AbstractExpressionNode* cond, AbstractStatementNode* bod, SymbolTable* symbolTable, uint32_t startLine) {
  condition = cond;
  body = bod;
  this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = bod->endLine;
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
ForStatementNode::ForStatementNode(AbstractStatementNode* init, AbstractStatementNode* upd, AbstractStatementNode* bod, AbstractExpressionNode* cond, SymbolTable* symbolTable, uint32_t startLine) {
  initialization = init;
  update = upd;
  body = bod;
  condition = cond;
  this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = bod->endLine;
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
NewAssignmentStatementNode::NewAssignmentStatementNode(std::string var, ParseDataType typ, AbstractExpressionNode* val, SymbolTable* symbolTable, uint32_t startLine) {
  
	variable = var;
	type = typ;
	value = val;
	this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = val->endLine;
  
  //add variable to table with correctly-typed dummy value    
  ParseData d;
  d.type = type;

	//array case should be handled in alternate constructor that takes subtype input
  symbolTable->declare(variable, d);
}

NewAssignmentStatementNode::NewAssignmentStatementNode(std::string var, ParseDataType typ, ParseDataType subTyp, AbstractExpressionNode* val, SymbolTable* symbolTable, uint32_t startLine) {
  
	variable = var;
	type = typ;
	subType = subTyp;
	value = val;
	this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = val->endLine;
  
  //add variable to table with correctly-typed dummy value    
  ParseData d;
  d.type = type;

	//if it's an array, add a subtype (this constructor should always be called for array type)
	if(typ == ARRAY_T) {
		Array* arr = (Array*) malloc(sizeof(Array));
		arr->subtype = subTyp;
		d.value.allocated = (void*) arr;
	}

  symbolTable->declare(variable, d);
}

NewAssignmentStatementNode::NewAssignmentStatementNode(std::string var, ParseDataType typ, SymbolTable* symbolTable, uint32_t startLine, uint32_t endLine) {
	variable = var;
	type = typ;
  value = NULL;
	this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = endLine;
  
  //add variable to table with correctly-typed dummy value    
  ParseData d;
  d.type = type;
  symbolTable->declare(variable, d);
}

NewAssignmentStatementNode::NewAssignmentStatementNode(std::string var, ParseDataType typ, ParseDataType subTyp, SymbolTable* symbolTable, uint32_t startLine, uint32_t endLine) {
	variable = var;
	type = typ;
	subType = subTyp;
  value = NULL;
	this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = endLine;
  
  //add variable to table with correctly-typed dummy value    
  ParseData d;
  d.type = type;

	//if it's an array, add a subtype (this constructor should always be called for array type)
	if(typ == ARRAY_T) {
		Array* arr = (Array*) malloc(sizeof(Array));
		arr->subtype = subTyp;
		d.value.allocated = (void*) arr;
	}
	
  symbolTable->declare(variable, d);
}

void NewAssignmentStatementNode::execute() {
	executeNewAssignmentStatement(this);
}

//represents existing variable assignment
AssignmentStatementNode::AssignmentStatementNode(std::string var, AbstractExpressionNode* val, SymbolTable* symbolTable, uint32_t startLine) {
  variable = var;
  value = val;
  this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = val->endLine;
}

void AssignmentStatementNode::execute() {
	executeAssignmentStatement(this);
}

//represents array index assignment, like arr[i] = 5
ArrayAssignmentStatementNode::ArrayAssignmentStatementNode(std::string var, AbstractExpressionNode* ind, AbstractExpressionNode* val, SymbolTable* symbolTable, uint32_t startLine) {
	variable = var;
	index = ind;
	value = val;
	this->symbolTable = symbolTable;
	this->startLine = startLine;
	this->endLine = val->endLine;
}

void ArrayAssignmentStatementNode::execute() {
	executeArrayAssignmentStatement(this);
}

//represents return statement in a function
ReturnStatementNode::ReturnStatementNode(AbstractExpressionNode* exp, bool* retFlag, ParseData* retVal, uint32_t startLine) {
	expression = exp;
	returnFlag = retFlag;
	returnValue = retVal;
	this->startLine = startLine;
	this->endLine = exp->endLine;
}

void ReturnStatementNode::execute() {
	executeReturnStatement(this);
}

//represents function declaration (and maybe definition)
FunctionStatementNode::FunctionStatementNode(std::string fName, Function* f, SymbolTable* symbolTable) {
	functionName = fName;
	function = f;
	this->symbolTable = symbolTable;

	ParseData d;
	d.type = FUN_T;
	d.value.allocated = f;
	symbolTable->declare(fName, d);
}

FunctionStatementNode::FunctionStatementNode(std::string fName, SymbolTable* symbolTable) {
	functionName = fName;
	function = NULL;
	this->symbolTable = symbolTable;

	ParseData d;
	d.type = FUN_T;
	symbolTable->declare(fName, d);
}

void FunctionStatementNode::execute() {
	executeFunctionStatement(this);
}


