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

static uint32_t index;
static vector<Token>* tokens;

/////////////////////////////////
//////    Access Tokens     /////
/////////////////////////////////

//return current Token and advance
Token* consume() {
  return (Token*) &(tokens->at(index++));
}

//examine the current Token, don't advance position
Token* peek() {
  return (Token*) &(tokens->at(index));
}

//examine a Token at given index
Token* peek(uint32_t inputIndex) {
  return (Token*) &(tokens->at(inputIndex));
}


/////////////////////////////////
//////    Subroutines     ///////
/////////////////////////////////

// variable, number, string, etc.
AbstractExpressionNode* evalLiteralGroup() {

  AbstractExpressionNode* next;
 
  if(peek()->type == LEFT_PAREN) {

    consume(); //consume left parenthesis
    next = evalExpression();

    if(peek()->type != RIGHT_PAREN) {
      //error
      cout << "ERROR LITERAL!!!!, index: " << index << endl;
    } else {
      consume(); //consume right parenthesis
    }

    return new GroupedExpressionNode(next);

  } else {

    Token* literal = consume();
    ParseData p = {typeTokenConversion(literal->type), literal->value};
    return new LiteralNode(p);
  }
}

//sign and bit/logical NOT
AbstractExpressionNode* evalSignNot() {
 
  if(isSignNotTokenType(peek()->type)) {

    ParseOperatorType op = unaryTokenConversion(consume()->type);
    AbstractExpressionNode* next = evalSignNot();

    if(typecheckUnaryExpression(op, next->evalType)) {
      return new UnaryOperatorNode(op, next);
    } else {
      cout << "ERROR SIGN NOT!" << endl;
      return NULL;
    }

  } else {

    return evalLiteralGroup();  
  }
}


// **
AbstractExpressionNode* evalExponent() {

  AbstractExpressionNode* head = evalSignNot();
  AbstractExpressionNode* next;

  while(peek()->type == EXPONENT) {
    consume(); 
    next = evalSignNot();

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

  TokenType t = peek()->type;
  
  switch(t) {
    
    case PRINTLN: {
      consume();
      return new PrintLineStatementNode(evalExpression());
    }
    
    case PRINT: {  
      consume();
      return new PrintStatementNode(evalExpression());
    }
    
    case LEFT_BRACE: {
      
      consume();
      vector<AbstractStatementNode*>* statements = new vector<AbstractStatementNode*>();
      while(peek()->type != RIGHT_BRACE) {
        statements->push_back(addStatement());  
      }
      consume(); //consume right brace
      
      return new GroupedStatementNode(statements);
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
          
          return new ConditionalStatementNode(cond, stat);
        }  
      }
      
      return new ConditionalStatementNode(cond, stat);
    }
    
    default: return new ExpressionStatementNode(evalExpression());
    
  }
  
}

//generate Abstract Syntax Tree from list of tokens
vector<AbstractStatementNode*>* parse(vector<Token>* tokenRef) {

  //set static variables to correct initial values
  index = 0;
  tokens = tokenRef;
  
  //create empty statement vector
  vector<AbstractStatementNode*>* statements = new vector<AbstractStatementNode*>();
  
  //append statement nodes until END is reached
  while(peek()->type != END) {
    statements->push_back(addStatement());
  }
  
  return statements;
}


