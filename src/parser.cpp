#include <iostream>
#include <cstdint>
#include <vector>
#include "token.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "typehandler.h"
#include "statementnode.h"
#include "parser.h"

using namespace std;

static uint32_t tokenIndex;
static vector<Token>* tokens;
static SymbolTable* symbolTable;

/////////////////////////////////
//////    Access Tokens     /////
/////////////////////////////////

//return current Token and advance
Token* consume() {
  return (Token*) &(tokens->at(tokenIndex++));
}

//examine the current Token, don't advance position
Token* peek() {
  return (Token*) &(tokens->at(tokenIndex));
}

//examine a Token at given index
Token* peekAhead(uint32_t offset) {
  return (Token*) &(tokens->at(tokenIndex+offset));
}


/////////////////////////////////
//////    Subroutines     ///////
/////////////////////////////////

// variable, number, string, etc.
AbstractExpressionNode* evalLiteralGroup() {

  AbstractExpressionNode* next;
 
  //if grouped expression
  if(peek()->type == LEFT_PAREN) {

    consume(); //consume left parenthesis
    next = evalExpression();

    if(peek()->type != RIGHT_PAREN) {
      //error
      cout << "ERROR LITERAL!!!!, index: " << tokenIndex << endl;
    } else {
      consume(); //consume right parenthesis
    }

    return new GroupedExpressionNode(next);

  } else if(peek()->type == VARIABLE) {
  
    Token* variable = consume();
    const char* name = variable->lexeme;
    uint32_t len = strlen(name);
    char* variableName = new char[len+1];
    strcpy(variableName, name);
    variableName[len] = '\0';
    
    return new VariableNode(variableName, symbolTable);
    
  } else {

    Token* literal = consume();
    ParseData p = {typeTokenConversion(literal->type), literal->value};
    return new LiteralNode(p);
  }
}

//member access
AbstractExpressionNode* evalMemberAccess() {
  
  AbstractExpressionNode* head = evalLiteralGroup();

  while(peek()->type == LEFT_BRACKET) {
  
    consume(); //consume [

    //if next value is colon, this is a slice with implicit start = 0
    if(peek()->type == COLON) {
  
      consume(); //consume :
      
      //start = 0
      ParseData startIndex;
      startIndex.type = INT32_T;
      startIndex.value.integer = 0;

      //if next value is ], implicit end = -1
      if(peek()->type == RIGHT_BRACKET) {
  
        consume(); //consume ]
        ParseData endIndex;
        endIndex.type = INT32_T;
        endIndex.value.integer = (int32_t) -1;
        
        head = new ArrayAccessNode(head, new LiteralNode(startIndex), new LiteralNode(endIndex));

      } else {
  
        //defined end index
        AbstractExpressionNode* end = evalExpression();

        if(peek()->type != RIGHT_BRACKET) {
          cout << "ERROR MEMBER ACCESS!!!" << endl;
          return NULL;

        } else {
    
          consume(); //consume ]
          head = new ArrayAccessNode(head, new LiteralNode(startIndex), end);
        }        

      }

    } else {
  
      //defined start index
      AbstractExpressionNode* start = evalExpression();

      //accessing a slice with explicit start
      if(peek()->type == COLON) {
    
        consume(); //consume :

        //end = -1 implied
        if(peek()->type == RIGHT_BRACKET) {
  
          consume(); //consume ]
          ParseData endIndex;
          endIndex.type = INT32_T;
          endIndex.value.integer = (int32_t) -1;
      
          head = new ArrayAccessNode(head, start, new LiteralNode(endIndex));

        } else {
  
          //defined end index
          AbstractExpressionNode* end = evalExpression();
            
          if(peek()->type != RIGHT_BRACKET) {
            cout << "ERROR MEMBER ACCESS!!!" << endl;
            return NULL;

          } else {
            consume(); //consume ]
            head = new ArrayAccessNode(head, start, end);
          }
        }

      } else {
        
        //accessing single value
        if(peek()->type != RIGHT_BRACKET) {
          cout << "ERROR MEMBER ACCESS!!!" << endl;
          return NULL;

        } else {
          consume(); //consume ]
          head = new ArrayAccessNode(head, start);
        }
      }

    }   

  }

  return head;
}


bool isCastStructure() {
  return peek()->type == LEFT_PAREN && isTypeTokenType(peekAhead(1)->type) && peekAhead(2)->type == RIGHT_PAREN;
}

//sign and bit/logical NOT
AbstractExpressionNode* evalCastSignNot() {
 
  //check if sign, not, or cast
  if(isSignNotTokenType(peek()->type)) {

    ParseOperatorType op = unaryTokenConversion(consume()->type);
    AbstractExpressionNode* next = evalCastSignNot();

    if(typecheckUnaryExpression(op, next->evalType)) {
      return new UnaryOperatorNode(op, next);
    } else {
      cout << "ERROR SIGN NOT!" << endl;
      return NULL;
    }

  } else if(isCastStructure()) {   
    
    consume(); //consume (
    ParseDataType finalType = typeTokenConversion(consume()->type);
    consume(); //consume )
    
    AbstractExpressionNode* next = evalCastSignNot();
    if(typecheckExplicitCastExpression(next->evalType, finalType)) {
      return new CastNode(next, finalType);
    } else {
      cout << "ERROR IMPROPER CAST!" << endl;
      return NULL;
    }
    
  } else {

    return evalMemberAccess();  
  }
}


// **
AbstractExpressionNode* evalExponent() {

  AbstractExpressionNode* head = evalCastSignNot();
  AbstractExpressionNode* next;

  while(peek()->type == EXPONENT) {
    consume(); 
    next = evalCastSignNot();

    if(typecheckArithmeticExpression(EXPONENT_OP, head->evalType, next->evalType)) {
      head = new ArithmeticOperatorNode(EXPONENT_OP, head, next);
    } else {
      cout << "ERROR EXPONENT!" << endl;
      return NULL;  
    }   

  }
  
  return head;
}


// * / %
AbstractExpressionNode* evalMultiplyDivideMod() {

  AbstractExpressionNode* head = evalExponent();
  AbstractExpressionNode* next;

  while(isMultiplyDivideModTokenType(peek()->type)) {
    ParseOperatorType op = binaryTokenConversion(consume()->type);
    next = evalExponent();

    if(typecheckArithmeticExpression(op, head->evalType, next->evalType)) {
      head = new ArithmeticOperatorNode(op, head, next);
    } else {
      cout << "ERROR MULTIPLY DIVIDE MOD!" << endl;
      return NULL;  
    }

  }

  return head;
}


// + -
AbstractExpressionNode* evalAddSubtract() {

  AbstractExpressionNode* head = evalMultiplyDivideMod();
  AbstractExpressionNode* next;

  while(isAddSubtractTokenType(peek()->type)) {
    ParseOperatorType op = binaryTokenConversion(consume()->type);
    next = evalMultiplyDivideMod();

    if(typecheckArithmeticExpression(op, head->evalType, next->evalType)) {
      head = new ArithmeticOperatorNode(op, head, next);
    } else {
      cout << "ERROR ADD SUB!" << endl;
      return NULL;  
    }
  }
  
  return head;
}

// << >>
AbstractExpressionNode* evalBitShift() {

  AbstractExpressionNode* head = evalAddSubtract();
  AbstractExpressionNode* next;

  while(isBitShiftTokenType(peek()->type)) {
    ParseOperatorType op = binaryTokenConversion(consume()->type);
    next = evalAddSubtract();

    if(typecheckBitLogicalExpression(op, head->evalType, next->evalType)) {
      head = new BitLogicalOperatorNode(op, head, next);
    } else {
      cout << "ERROR SHIFT!" << endl;
      return NULL;  
    }
  }
  
  return head;
}


// < <= > >=
AbstractExpressionNode* evalComparison() {

  AbstractExpressionNode* head = evalBitShift();
  AbstractExpressionNode* next;

  while(isInequalityTokenType(peek()->type)) {
    ParseOperatorType op = binaryTokenConversion(consume()->type);
    next = evalBitShift();

    if(typecheckComparisonExpression(op, head->evalType, next->evalType)) {
      head = new ComparisonOperatorNode(op, head, next);
    } else {
      cout << "ERROR INEQ!" << endl;
      return NULL;  
    }
  }
  
  return head;
}

// == and !=
AbstractExpressionNode* evalEquality() {

  AbstractExpressionNode* head = evalComparison();
  AbstractExpressionNode* next;

  while(isEqualityTokenType(peek()->type)) {
    ParseOperatorType op = binaryTokenConversion(consume()->type);
    next = evalComparison();

    if(typecheckComparisonExpression(op, head->evalType, next->evalType)) {
      head = new ComparisonOperatorNode(op, head, next);
    } else {
      cout << "ERROR EQ!" << endl;
      return NULL;  
    }
  }
 
  return head;
}

// &
AbstractExpressionNode* evalBitAnd() {

  AbstractExpressionNode* head = evalEquality();
  AbstractExpressionNode* next;

  while(peek()->type == BIT_AND) {
    consume();
    next = evalEquality();

    if(typecheckBitLogicalExpression(BIT_AND_OP, head->evalType, next->evalType)) {
      head = new BitLogicalOperatorNode(BIT_AND_OP, head, next);
    } else {
      cout << "ERROR BIT AND!" << endl;
      return NULL;  
    }
  }
 
  return head;
}


// ^
AbstractExpressionNode* evalBitXor() {

  AbstractExpressionNode* head = evalBitAnd();
  AbstractExpressionNode* next;

  while(peek()->type == BIT_XOR) {
    consume();
    next = evalBitAnd();

    if(typecheckBitLogicalExpression(BIT_XOR_OP, head->evalType, next->evalType)) {
      head = new BitLogicalOperatorNode(BIT_XOR_OP, head, next);
    } else {
      cout << "ERROR BIT XOR!" << endl;
      return NULL;  
    }
  }
 
  return head;
}

// |
AbstractExpressionNode* evalBitOr() {

  AbstractExpressionNode* head = evalBitXor();
  AbstractExpressionNode* next;

  while(peek()->type == BIT_OR) {
    consume();
    next = evalBitXor();

    if(typecheckBitLogicalExpression(BIT_OR_OP, head->evalType, next->evalType)) {
      head = new BitLogicalOperatorNode(BIT_OR_OP, head, next);
    } else {
      cout << "ERROR BIT OR!" << endl;
      return NULL;  
    }
  }
 
  return head;
}


// &&
AbstractExpressionNode* evalLogicAnd() {

  AbstractExpressionNode* head = evalBitOr();
  AbstractExpressionNode* next;

  while(peek()->type == AND) {
    consume();
    next = evalBitOr();

    if(typecheckBitLogicalExpression(AND_OP, head->evalType, next->evalType)) {
      head = new BitLogicalOperatorNode(AND_OP, head, next);
    } else {
      cout << "ERROR AND!" << endl;
      return NULL;  
    }
  }
 
  return head;
}


// ^^
AbstractExpressionNode* evalLogicXor() {

  AbstractExpressionNode* head = evalLogicAnd();
  AbstractExpressionNode* next;

  while(peek()->type == XOR) {
    consume();
    next = evalLogicAnd();

    if(typecheckBitLogicalExpression(XOR_OP, head->evalType, next->evalType)) {
      head = new BitLogicalOperatorNode(XOR_OP, head, next);
    } else {
      cout << "ERROR XOR!" << endl;
      return NULL;  
    }
  }
 
  return head;
}

// ||
AbstractExpressionNode* evalLogicOr() {

  AbstractExpressionNode* head = evalLogicXor();
  AbstractExpressionNode* next;

  while(peek()->type == OR) {
    consume();
    next = evalLogicXor();

    if(typecheckBitLogicalExpression(OR_OP, head->evalType, next->evalType)) {
      head = new BitLogicalOperatorNode(OR_OP, head, next);
    } else {
      cout << "ERROR OR!" << endl;
      return NULL;  
    }
  }
 
  return head;
}


//  = += -= *= **= /= &= ^= |= <<= >>=
AbstractExpressionNode* evalAssignment() {
  return evalLogicOr();
  
}

AbstractExpressionNode* evalExpression() {
  return evalLogicOr();
}


AbstractStatementNode* addStatement() {

  Token* t = peek();
  
  //first check for declaration and assignment
  if(isTypeTokenType(t->type)) {
   
    //variable declaration
    ParseDataType type = typeTokenConversion(t->type);
    consume(); //consume type Token    
    
    //get variable name
    const char* constVariable = consume()->lexeme;
    uint32_t len = strlen(constVariable);
    char* variable = new char[len+1];
    strcpy(variable, constVariable);
    variable[len] = '\0';
    
    //make sure variable is not already declared
    if(symbolTable->isDeclaredInScope(variable)) {
      std::cout << "ERROR: variable " << variable << " already declared!" << std::endl;
      return NULL;
    }
    
    //if being assigned
    if(peek()->type == EQ) {
      
      consume(); //consume = Token
      AbstractExpressionNode* expression = evalExpression();
      
      //check implicit casting validity
      if(!typecheckImplicitCastExpression(expression->evalType, type)) {
        std::cout << "ERROR: invalid implicit type cast" << std::endl;
        return NULL;
      } else {
        AbstractStatementNode* troll =  new NewAssignmentStatementNode(variable, type, expression, symbolTable);
        return troll;
      }
      
    } else {  
      //no initial value
      return new NewAssignmentStatementNode(variable, type, symbolTable);
    }
    
  } else if(t->type == VARIABLE) {
    
    //existing variable assignment
    
    //get variable name
    const char* constVariable = consume()->lexeme;
    uint32_t len = strlen(constVariable);
    char* variable = new char[len+1];
    strcpy(variable, constVariable);
    variable[len] = '\0';
    
    //make sure variable is already declared in some scope
    if(!symbolTable->isDeclared(variable)) {
      std::cout << "ERROR: variable is not yet declared!" << std::endl;
      return NULL;
    }
    
    //get variable type from the symbol table
    ParseDataType type = (symbolTable->get(variable)).type;
    
    if(peek()->type != EQ) {
      std::cout << "'=' must follow variable name when assigning value" << std::endl;
      return NULL;
    } else {
      consume(); //consume = 
    }
    
    AbstractExpressionNode* expression = evalExpression();
    
    //make sure implicit cast is valid
    if(!typecheckImplicitCastExpression(expression->evalType, type)) {
      std::cout << "ERROR: invalid implicit type cast" << std::endl;
      return NULL;
    } else {
      return new AssignmentStatementNode(variable, expression, symbolTable);
    }
  }
	
  switch(t->type) {
    
    case PRINTLN: {
      consume();
      return new PrintLineStatementNode(evalExpression(), symbolTable);
    }
    
    case PRINT: {  
      consume();
      return new PrintStatementNode(evalExpression(), symbolTable);
    }
    
    case LEFT_BRACE: {
      consume();
      vector<AbstractStatementNode*>* statements = new vector<AbstractStatementNode*>();
      
      //enter a new scope in symbol table (for static scope-checking)
      symbolTable->enterNewScope();
      
      while(peek()->type != RIGHT_BRACE) {
        statements->push_back(addStatement());  
      }
      
      consume(); //consume right brace
      
      //leave scope
      symbolTable->leaveScope();
      
      return new GroupedStatementNode(statements, symbolTable);
    }
    
    case IF: {
    
      vector<AbstractExpressionNode*>* cond = new vector<AbstractExpressionNode*>();
      vector<AbstractStatementNode*>* stat = new vector<AbstractStatementNode*>();
      
      //consume the IF token and add first condition-statement pair
      consume();
      AbstractExpressionNode* e = evalExpression();
      
      if(e->evalType != BOOL_T) {
        cout << "Conditional must take a boolean expression" << endl;
        return NULL;
      } else {
        cond->push_back(e);
        stat->push_back(addStatement());
      }
      
      //now append to the chain as needed
      while(peek()->type == ELIF || peek()->type == ELSE) {
        
        if(peek()->type == ELIF) {
          
          //consume ELIF token and add condition-statement pair
          consume();
          AbstractExpressionNode* exp = evalExpression();
          
          if(exp->evalType != BOOL_T) {
            cout << "Conditional must take a boolean expression" << endl;
            return NULL;
          } else {
            cond->push_back(exp);
            stat->push_back(addStatement());
          }
           
        } else {
          
          //consume ELSE token and add dummy true-statement pair
          consume();
          
          ParseData d;
          d.type = BOOL_T; d.value.integer = true;
          LiteralNode* exp = new LiteralNode(d);
          
          cond->push_back(exp);
          stat->push_back(addStatement());
          
          return new ConditionalStatementNode(cond, stat, symbolTable);
        }  
      }
      
      return new ConditionalStatementNode(cond, stat, symbolTable);
    }
    
    default: return new ExpressionStatementNode(evalExpression(), symbolTable);
    
  }
  
}

//generate Abstract Syntax Tree from list of tokens
vector<AbstractStatementNode*>* parse(vector<Token>* tokenRef) {

  //set static variables to correct initial values
  tokenIndex = 0;
  tokens = tokenRef;
  symbolTable = new SymbolTable();
  
  //create empty statement vector
  vector<AbstractStatementNode*>* statements = new vector<AbstractStatementNode*>();
  
  //append statement nodes until END is reached
  while(peek()->type != END) {
    statements->push_back(addStatement());
  }
  
  return statements;
}


