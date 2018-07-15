#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>
#include "utils.h"
#include "errors.h"
#include "token.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "typehandler.h"
#include "statementnode.h"
#include "parser.h"
#include "function.h"
#include "array.h"

using namespace std;

static uint32_t tokenIndex;
static vector<Token>* tokens;
static SymbolTable* symbolTable;
static vector<char*>* codeLines;

static vector<bool*> returnFlag;
static vector<ParseData*> returnValue;

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

char* getCodeLineBlock(uint32_t startIndex, uint32_t endIndex) {

  //append all lines to a string object
  string str;
  for(uint32_t i = startIndex; i <= endIndex; i++) {
    str.append("\t");
    str.append(codeLines->at(i));
  }
  
  //return deep copy
  return copyString(str.c_str());
}

/////////////////////////////////
//////    Subroutines     ///////
/////////////////////////////////

// variable, number, string, etc.
AbstractExpressionNode* evalLiteralGroup() {

  AbstractExpressionNode* next;
 
  //if grouped expression
  if(peek()->type == LEFT_PAREN) {

    Token* startGroupToken = consume(); //consume left parenthesis
    Token* endGroupToken;
    
    next = evalExpression();

    if(peek()->type != RIGHT_PAREN) {
      //!!! parse syntax error
      throw ParseSyntaxError(startGroupToken->line+1, codeLines->at(startGroupToken->line), "(", "The parenthesized expression is missing a ')'");
    } else {
      endGroupToken = consume(); //consume right parenthesis
    }

    return new GroupedExpressionNode(next, startGroupToken->line, endGroupToken->line);


	} else if(peek()->type == LEFT_BRACKET) {

		//initialized array
		Token* leftBracketToken = consume();

		//insert the initial values into a list
		vector<AbstractExpressionNode*> initValues;
		while(peek()->type != RIGHT_BRACKET) {

			initValues.push_back(evalExpression());

			if(peek()->type == COMMA) {
				consume(); //consume , token
			} else if(peek()->type == RIGHT_BRACKET) {
				//do nothing, deal with it outside the while loop
			} else {
				//throw error if initializer list is not terminated
				throw ParseSyntaxError(leftBracketToken->line+1, peek()->line+1, getCodeLineBlock(leftBracketToken->line, peek()->line), 
																	"Expected ']' to terminate array initializer list");
			}
		}

		Token* rightBracketToken = consume(); //consume ]

		uint32_t length = (uint32_t) initValues.size();
		AbstractExpressionNode** values = (AbstractExpressionNode**) malloc(sizeof(AbstractExpressionNode*) * length);

		//copy references out of list into array
		//TODO: typehandling
		for(uint32_t i = 0; i < length; i++) {
			values[i] = initValues[i];
		}

		//convert fixed length into a LiteralNode
		ParseData len;
		len.type = UINT32_T;
		len.value.integer = length;

		//TODO: find a better way to resolve array type
		return new ArrayNode(values[0]->evalType, true, new LiteralNode(len, -1), values, leftBracketToken->line, rightBracketToken->line);

	} else if(peek()->type == NEW) {

		//array or object instantiation
		Token* newToken = consume();

		if(!isTypeTokenType(peek()->type)) {
			throw ParseSyntaxError(newToken->line+1, getCodeLineBlock(newToken->line, newToken->line), "Expected type specifier as part of array instantiation");
		}

		ParseDataType type = ARRAY_T;
		Token* subtypeToken = consume();
		ParseDataType subtype = typeTokenConversion(subtypeToken->type);

		if(peek()->type != LEFT_BRACKET) {
			throw ParseSyntaxError(newToken->line+1, subtypeToken->line+1, getCodeLineBlock(newToken->line, subtypeToken->line), "Expected '[' as part of array instantiation");
		}

		Token* leftBracketToken = consume();
		AbstractExpressionNode* length = evalExpression();

		//make sure length expression is an integer
		if(!isIntParseDataType(length->evalType)) {

			string message = "Expected integer/char type for array length specifier, not ";
			message.append(toStringParseDataType(length->evalType));

			throw StaticTypeError(leftBracketToken->line+1, length->endLine+1, getCodeLineBlock(leftBracketToken->line, length->endLine), 
														copyString(message.c_str()));
		}


		if(peek()->type != RIGHT_BRACKET) {
			throw ParseSyntaxError(newToken->line+1, length->endLine+1, getCodeLineBlock(newToken->line, length->endLine+1), "Expected ']' as part of array instantiation");
		}

		Token* rightBracketToken = consume();

		//now return ArrayNode with appropriate information
		return new ArrayNode(subtype, false, length, NULL, newToken->line, rightBracketToken->line);

  } else if(peek()->type == VARIABLE) {

		Token* variable = consume();

		//make sure variable is already declared
		string variableName((char*) variable->lexeme);
		if(false && !symbolTable->isDeclared(variableName)) {
			throw StaticVariableScopeError(variable->line+1, variable->lexeme, getCodeLineBlock(variable->line, variable->line), false);
		}

		//get variable type
		ParseDataType varType = symbolTable->get(variableName).type;

		if(peek()->type == LEFT_PAREN) {

			//function call case
			Token* leftParenToken = consume(); //consume (

			//make sure this variable is of function type
			if(varType != FUN_T) {
				throw StaticTypeError(variable->line+1, leftParenToken->line+1, getCodeLineBlock(variable->line, leftParenToken->line), "function call", varType);
			}
			
			//get function information
			Function* function = (Function*) (symbolTable->get(variableName).value.allocated);
			uint32_t numArgs = function->numArgs;
			ParseDataType* argTypes = function->argTypes;

			//read in arguments
			AbstractExpressionNode** arguments = (AbstractExpressionNode**) malloc(sizeof(AbstractExpressionNode*) * numArgs);
			Token* lastCommaToken;

			for(uint32_t i = 0; i < numArgs; i++) {

				//if next item is not an argument, throw error
				if(peek()->type == END) {
					uint32_t startLine = variable->line+1;
					uint32_t endLine = (i > 0) ? arguments[i-1]->endLine+1 : leftParenToken->line+1;
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Too few arguments provided to this function call");
				}

				//get next argument
				arguments[i] = evalExpression();

				//make sure argument is of the right type
				if(!typecheckImplicitCastExpression(arguments[i]->evalType, argTypes[i])) {

					uint32_t startLine = variable->line+1;
					uint32_t endLine = arguments[i]->endLine+1;

					string message = "Cannot implicitly convert a ";
					message.append(toStringParseDataType(arguments[i]->evalType));
					message.append(" argument to ");
					message.append(toStringParseDataType(argTypes[i]));
					message.append(" type");

					throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), copyString(message.c_str()));
				}

				if(i < numArgs-1) {
					if(peek()->type != COMMA) {
						throw ParseSyntaxError(variable->line+1, arguments[i]->endLine+1, getCodeLineBlock(variable->line, arguments[i]->endLine),
																	"Expected comma to separate function arguments");
					}
					lastCommaToken = consume(); //consume ,
				}
			}

			if(peek()->type != RIGHT_PAREN) {
				throw ParseSyntaxError(variable->line+1, lastCommaToken->line+1, getCodeLineBlock(variable->line, lastCommaToken->line),
															"Expected ')' to terminate argument list");
			}
			consume(); //consume )

			//now return FunctionExpressionNode
			return new FunctionExpressionNode(numArgs, arguments, function, symbolTable);

		} else {

			//simple value substitution case
			return new VariableNode(variableName, symbolTable, variable->line);
		}
  
  } else {

    Token* literal = consume();
    ParseData p = {typeTokenConversion(literal->type), literal->value};
    return new LiteralNode(p, literal->line);
  }
}

//member access (strings and arrays)
bool isPostfixStructure() {
  return (peek()->type == ADD && peekAhead(1)->type == ADD) || (peek()->type == SUBTRACT && peekAhead(1)->type == SUBTRACT);
}

AbstractExpressionNode* handleMemberAccess(AbstractExpressionNode* head) {

  if(head->evalType != STRING_T && head->evalType != ARRAY_T) {
      
    //first write message
    const char* arrayAccessErrorMessage = "A value must be of array or string type to be indexed";
    throw StaticTypeError(head->startLine+1, peek()->line+1, getCodeLineBlock(head->startLine, peek()->line), arrayAccessErrorMessage);
  }
  
  Token* leftBracketToken = consume(); //consume [

  //if next value is colon, this is a slice with implicit start = 0
  if(peek()->type == COLON) {

    Token* colonToken = consume(); //consume :
    
    //start = 0
    ParseData startIndex;
    startIndex.type = INT32_T;
    startIndex.value.integer = 0;

    //if next value is ], implicit end = -1
    if(peek()->type == RIGHT_BRACKET) {

      Token* rightBracketToken = consume(); //consume ]
      ParseData endIndex;
      endIndex.type = INT32_T;
      endIndex.value.integer = (int32_t) -1;
      
      return new ArrayAccessNode(head, new LiteralNode(startIndex, leftBracketToken->line), 
                                  new LiteralNode(endIndex, rightBracketToken->line), rightBracketToken->line);

    } else {

      //defined end index
      AbstractExpressionNode* end = evalExpression();
      
      //make sure that the expression evaluates to an integer
      if(!typecheckMemberAccessExpression(end->evalType)) {
        throw StaticTypeError(head->startLine+1, end->endLine+1, getCodeLineBlock(head->startLine, end->endLine), "array indexing", end->evalType);
      }

      if(peek()->type != RIGHT_BRACKET) {
        
        //!!! If [ does not have a corresponding ], throw an error
        throw ParseSyntaxError(leftBracketToken->line+1, codeLines->at(leftBracketToken->line), "[", "Array access operation must end with ']'");
        
      } else {
  
        Token* rightBracketToken = consume(); //consume ]
        return new ArrayAccessNode(head, new LiteralNode(startIndex, leftBracketToken->line), end, rightBracketToken->line);
      }
    }

  } else {

    //defined start index
    AbstractExpressionNode* start = evalExpression();
    
    //make sure that the expression evaluates to an integer
    if(!typecheckMemberAccessExpression(start->evalType)) {
      throw StaticTypeError(head->startLine+1, start->endLine+1, getCodeLineBlock(head->startLine, start->endLine), "array indexing", start->evalType);
    }

    //accessing a slice with explicit start
    if(peek()->type == COLON) {
  
      consume(); //consume :

      //end = -1 implied
      if(peek()->type == RIGHT_BRACKET) {

        Token* rightBracketToken = consume(); //consume ]
        ParseData endIndex;
        endIndex.type = INT32_T;
        endIndex.value.integer = (int32_t) -1;
    
        return new ArrayAccessNode(head, start, new LiteralNode(endIndex, rightBracketToken->line), rightBracketToken->line);

      } else {

        //defined end index
        AbstractExpressionNode* end = evalExpression();
        
        //make sure that the expression evaluates to an integer
        if(!typecheckMemberAccessExpression(end->evalType)) {
          throw StaticTypeError(head->startLine+1, end->endLine+1, getCodeLineBlock(head->startLine, end->endLine), "array indexing", end->evalType);            
        }
          
        if(peek()->type != RIGHT_BRACKET) {
          
          //If [ does not have a corresponding ], throw an error
          throw ParseSyntaxError(leftBracketToken->line+1, codeLines->at(leftBracketToken->line), "[", "Array access operation must end with ']'");

        } else {
          Token* rightBracketToken = consume(); //consume ]
          return new ArrayAccessNode(head, start, end, rightBracketToken->line);
        }
      }

    } else {
      
      //accessing single value
      if(peek()->type != RIGHT_BRACKET) {
        
        //!!! If [ does not have a corresponding ], throw an error
        throw ParseSyntaxError(leftBracketToken->line+1, codeLines->at(leftBracketToken->line), "[", "Array access operation must end with ']'");

      } else {
        Token* rightBracketToken = consume(); //consume ]
        return new ArrayAccessNode(head, start, rightBracketToken->line);
      }
    }
  }

  return NULL;
}

AbstractExpressionNode* evalPostfixMemberAccess() {
  
  AbstractExpressionNode* head = evalLiteralGroup();
  VariableNode* varNode = dynamic_cast<VariableNode*>(head);

  if(varNode && isPostfixStructure()) {

		string variableName(varNode->variable);

    //make sure variable is declared
    if(!symbolTable->isDeclared(variableName)) {
      throw StaticVariableScopeError(varNode->startLine+1, varNode->variable, getCodeLineBlock(varNode->startLine, varNode->startLine), false);   
    }

    //get variable type
    ParseDataType varType = varNode->evalType;

    Token* firstPlus = consume(); //consume first + or -
    Token* secondPlus = consume(); //consume second + or -

    if(secondPlus->type == ADD) {
      //make sure type is correct
      if(!typecheckUnaryExpression(POSTFIX_INC_OP, varType)) {
        throw StaticTypeError(varNode->startLine+1, secondPlus->line+1, getCodeLineBlock(varNode->startLine, secondPlus->line), "postfix increment", varType);
      }
      return new UnaryOperatorNode(POSTFIX_INC_OP, varNode, symbolTable, secondPlus->line);

    } else {
      //make sure type is correct
      if(!typecheckUnaryExpression(POSTFIX_DEC_OP, varType)) {
        throw StaticTypeError(varNode->startLine+1, secondPlus->line+1, getCodeLineBlock(varNode->startLine, secondPlus->line), "postfix decrement", varType);
      }
      return new UnaryOperatorNode(POSTFIX_DEC_OP, varNode, symbolTable, secondPlus->line); 
    }
  }

  while(peek()->type == LEFT_BRACKET) {
    head = handleMemberAccess(head);
  }

  return head;
}


bool isCastStructure() {
  return peek()->type == LEFT_PAREN && isTypeTokenType(peekAhead(1)->type) && peekAhead(2)->type == RIGHT_PAREN;
}

bool isPrefixStructure() {
  return (peek()->type == ADD && peekAhead(1)->type == ADD) || (peek()->type == SUBTRACT && peekAhead(1)->type == SUBTRACT);
}

//sign and bit/logical NOT
AbstractExpressionNode* evalPrefixCastSignNot() {
 
  //check if prefix
  if(isPrefixStructure()) {

    Token* firstPlus = consume();
    Token* secondPlus = consume(); //consume second +

    AbstractExpressionNode* head = evalPostfixMemberAccess();
    VariableNode* varNode = dynamic_cast<VariableNode*>(head);

    //if it's not a variable, throw a syntax error
    if(!varNode) {
      throw ParseSyntaxError(firstPlus->line+1, head->endLine+1, getCodeLineBlock(firstPlus->line, head->endLine), "Prefix operator expects variable argument");
    }

    ParseDataType type = varNode->evalType;
    ParseOperatorType op = (firstPlus->type == ADD) ? PREFIX_INC_OP : PREFIX_DEC_OP;

    //if it's not a number, throw a type error
    if(!typecheckUnaryExpression(op, type)) {
      throw StaticTypeError(firstPlus->line+1, head->endLine+1, getCodeLineBlock(firstPlus->line, head->endLine), (op == PREFIX_INC_OP) ? "prefix increment" : "prefix decrement", type);
    }

    return new UnaryOperatorNode(op, varNode, symbolTable, firstPlus->line);

  } else if(isSignNotTokenType(peek()->type)) {

    Token* signNotToken = consume();
    ParseOperatorType op = unaryTokenConversion(signNotToken->type);
    AbstractExpressionNode* next = evalPrefixCastSignNot();

    if(typecheckUnaryExpression(op, next->evalType)) {
      return new UnaryOperatorNode(op, next, signNotToken->line);
    } else {
      //if type mismatch, throw error
      throw StaticTypeError(signNotToken->line+1, next->endLine+1, getCodeLineBlock(signNotToken->line, next->endLine), toWordParseOperatorType(op), next->evalType);
    }

  } else if(isCastStructure()) {   
    
    Token* leftParenToken = consume(); //consume (
    ParseDataType finalType = typeTokenConversion(consume()->type);
    consume(); //consume )
    
    AbstractExpressionNode* next = evalPrefixCastSignNot();
    if(typecheckExplicitCastExpression(next->evalType, finalType)) {
      return new CastNode(next, finalType, leftParenToken->line);
    } else {
      throw StaticCastError(leftParenToken->line+1, next->endLine+1, getCodeLineBlock(leftParenToken->line, next->endLine), next->evalType, finalType, true);
    }
    
  } else {

    return evalPostfixMemberAccess();  
  }
}


// **
AbstractExpressionNode* evalExponent() {

  AbstractExpressionNode* head = evalPrefixCastSignNot();
  AbstractExpressionNode* next;

  while(peek()->type == EXPONENT) {
    consume(); 
    next = evalPrefixCastSignNot();

    if(typecheckArithmeticExpression(EXPONENT_OP, head->evalType, next->evalType)) {
      head = new ArithmeticOperatorNode(EXPONENT_OP, head, next);
    } else {
      //!!!
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), "exponentiation", head->evalType, next->evalType);
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
      //!!!
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), toWordParseOperatorType(op), head->evalType, next->evalType);
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
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), toWordParseOperatorType(op), head->evalType, next->evalType);
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
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), toWordParseOperatorType(op), head->evalType, next->evalType);
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
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), toWordParseOperatorType(op), head->evalType, next->evalType); 
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
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), toWordParseOperatorType(op), head->evalType, next->evalType);  
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
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), "bitwise AND", head->evalType, next->evalType); 
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
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), "bitwise XOR", head->evalType, next->evalType); 
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
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), "bitwise OR", head->evalType, next->evalType);
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
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), "logical AND", head->evalType, next->evalType);  
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
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), "logical XOR", head->evalType, next->evalType); 
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
      throw StaticTypeError(head->startLine+1, next->endLine+1, getCodeLineBlock(head->startLine, next->endLine), "logical OR", head->evalType, next->evalType);  
    }
  }
 
  return head;
}


//  = += -= *= **= /= &= ^= |= <<= >>=
AbstractExpressionNode* evalAssignment() {
  
  AbstractExpressionNode* head = evalLogicOr();
  VariableNode* var = dynamic_cast<VariableNode*>(head);
  
  if(var && isAssignmentOperatorTokenType(peek()->type)) {
  
    //make sure the variable is already declared in some scope
    if(!symbolTable->isDeclared(var->variable)) {
      throw StaticVariableScopeError(var->startLine+1, var->variable, getCodeLineBlock(var->startLine, var->startLine), false);
    }
    
    //get variable's type
    ParseDataType type = var->evalType;
    
    Token* assignmentToken = consume();
    AbstractExpressionNode* next = evalAssignment();
		
		//make sure the assignment operation is even valid
		if(assignmentToken->type != EQ) {

			ParseOperatorType opType;
			switch(assignmentToken->type) {
				case ADD_EQ: opType = ADD_OP; break;
				case SUBTRACT_EQ: opType = SUBTRACT_OP; break;
				case EXPONENT_EQ: opType = EXPONENT_OP; break;
				case MULTIPLY_EQ: opType = MULTIPLY_OP; break;
				case DIVIDE_EQ: opType = DIVIDE_OP; break;
				case AND_EQ: opType = AND_OP; break;
				case XOR_EQ: opType = XOR_OP; break;
				case OR_EQ: opType = OR_OP; break;
				default: opType = INVALID_OP;
			}

			if(isArithmeticParseOperatorType(opType)) {

				if(!typecheckArithmeticExpression(opType, type, next->evalType)) {
					throw StaticTypeError(var->startLine+1, next->endLine+1, getCodeLineBlock(var->startLine, next->endLine), 
																toStringParseOperatorType(opType), type, next->evalType); 
				}

			} else {

				if(!typecheckBitLogicalExpression(opType, type, next->evalType)) {
					throw StaticTypeError(var->startLine+1, next->endLine+1, getCodeLineBlock(var->startLine, next->endLine), 
																toStringParseOperatorType(opType), type, next->evalType); 
				}
			}
		}
    
    switch(assignmentToken->type) {
      case EQ: break;
      case ADD_EQ: next = new ArithmeticOperatorNode(ADD_OP, var, next); break; 
      case SUBTRACT_EQ: next = new ArithmeticOperatorNode(SUBTRACT_OP, var, next); break;
      case EXPONENT_EQ: next = new ArithmeticOperatorNode(EXPONENT_OP, var, next); break;
      case MULTIPLY_EQ: next = new ArithmeticOperatorNode(MULTIPLY_OP, var, next); break;
      case DIVIDE_EQ: next = new ArithmeticOperatorNode(DIVIDE_OP, var, next); break;
      case AND_EQ: {
        if(type == BOOL_T)
          next = new BitLogicalOperatorNode(AND_OP, var, next);
        else 
          next = new BitLogicalOperatorNode(BIT_AND_OP, var, next);
        break;
      }
      
      case XOR_EQ: {
        if(type == BOOL_T)
          next = new BitLogicalOperatorNode(XOR_OP, var, next);
        else 
          next = new BitLogicalOperatorNode(BIT_XOR_OP, var, next);
        break;
      }
      
      case OR_EQ: {
        if(type == BOOL_T)
          next = new BitLogicalOperatorNode(OR_OP, var, next);
        else 
          next = new BitLogicalOperatorNode(BIT_OR_OP, var, next);
        break;
      }
    }
    
    //now make sure that implicit type cast is valid
    if(!typecheckImplicitCastExpression(next->evalType, type)) {
      throw StaticCastError(var->startLine+1, next->endLine+1, getCodeLineBlock(var->startLine, next->endLine), next->evalType, type, false);
    }
    
    return new AssignmentExpressionNode(var->variable, type, next, symbolTable, var->startLine);
    
  } else {
    return head;
  }
  
}

AbstractExpressionNode* evalExpression() {
  return evalAssignment();
}

AbstractStatementNode* addStatement() {

  Token* t = peek();
  
  //first check for declaration and assignment
	Token* leftBracket;
	Token* rightBracket;

  if(isTypeTokenType(t->type)) {
   
    //variable declaration
    ParseDataType type = typeTokenConversion(t->type);

		//only in the case of an array
		ParseDataType subtype;

    Token* typeToken = consume(); //consume type Token

		//check if it is an array type
		if(peek()->type == LEFT_BRACKET) {

			leftBracket = consume();

			if(peek()->type != RIGHT_BRACKET) {
				Token* invalidToken = consume();

				if(invalidToken->type == END) {
					throw ParseSyntaxError(typeToken->line+1, invalidToken->line+1, getCodeLineBlock(typeToken->line, invalidToken->line),
														"Expected ']' as part of array type declaration");
				} else {
					throw ParseSyntaxError(typeToken->line+1, invalidToken->line+1, getCodeLineBlock(typeToken->line, invalidToken->line),
																invalidToken->lexeme, "Expected ']' as part of array type declaration");
				}
			}

			rightBracket = consume();

			//update typing
			subtype = type;
			type = ARRAY_T;
		}    
    
    //get variable name
    Token* variableToken = consume();
		string variable((char*) variableToken->lexeme);

		//make sure the token is an actual variable
		if(variableToken->type != VARIABLE) {
			if(variableToken->type == END) {
				throw ParseSyntaxError(typeToken->line+1, getCodeLineBlock(typeToken->line, typeToken->line), "Expected variable name in declaration");
			} else {
				throw ParseSyntaxError(typeToken->line+1, variableToken->line+1, getCodeLineBlock(typeToken->line, variableToken->line), "Expected variable name in declaration");
			}
		}

    //make sure variable is not already declared
    if(symbolTable->isDeclaredInScope(variable)) {
      throw StaticVariableScopeError(variableToken->line+1, variableToken->lexeme, getCodeLineBlock(variableToken->line, variableToken->line), true); 
    }
    
    //if being assigned an initial value
    if(peek()->type == EQ) {
      
      Token* equalToken = consume(); //consume = Token
      AbstractExpressionNode* expression = evalExpression();

      //check implicit casting validity
			if(type == ARRAY_T) {

				if(expression->evalType != ARRAY_T) {
					uint32_t startLine = typeToken->line+1;
					uint32_t endLine = equalToken->line+1;
					throw StaticCastError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), expression->evalType, ARRAY_T, false);
				}

				//make sure array members can be implicitly cast to type of array being assigned to
				if(!typecheckImplicitCastExpression(expression->subType, subtype)) {

					uint32_t startLine = typeToken->line+1;
					uint32_t endLine = equalToken->line+1;

					string message = "Cannot implicitly cast array members of type ";
					message.append(toStringParseDataType(expression->subType));
					message.append(" to type ");
					message.append(toStringParseDataType(subtype));

					throw StaticCastError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), copyString(message.c_str()));
				}

				//return 	NewAssignmentStatementNode with specified expression
				return new NewAssignmentStatementNode(variable, type, expression, symbolTable);
			}

			//non-array case
      if(!typecheckImplicitCastExpression(expression->evalType, type)) {
        throw StaticCastError(variableToken->line+1, expression->endLine+1, getCodeLineBlock(variableToken->line, expression->endLine), expression->evalType, type, false); 
      } else {
        return new NewAssignmentStatementNode(variable, type, expression, symbolTable);
      }
      
    } else {  
      //no initial value (IMPOSSIBLE for array for now)
      return new NewAssignmentStatementNode(variable, type, symbolTable);
    }
    
  } else if(t->type == VARIABLE) {
    
    //existing variable assignment
    
    //get variable name
    Token* varToken = consume();
    string variable(varToken->lexeme);
    
    //make sure variable is already declared in some scope
    if(!symbolTable->isDeclared(variable)) {
      throw StaticVariableScopeError(varToken->line+1, varToken->lexeme, getCodeLineBlock(varToken->line, varToken->line), false);
    }
    
    //get variable type (and maybe array element subtype) from the symbol table
		ParseDataType type;
		ParseDataType subtype = INVALID_T;

    type = (symbolTable->get(variable)).type;
		if(type == ARRAY_T) {
			subtype = ((Array*) (symbolTable->get(variable)).value.allocated)->subtype;
		}

		//initial value of variable being assigned to (in event of *=, -=, etc.)
		AbstractExpressionNode* initValue = NULL;

		//deal with array index assignment possibility
		AbstractExpressionNode* arrIndex;
		if(peek()->type == LEFT_BRACKET) {

			Token* leftBracketToken = consume();

			//make sure it is an array
			if(type != ARRAY_T) {
				uint32_t startLine = varToken->line+1;
				uint32_t endLine = leftBracketToken->line+1;
				throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "array index assignment", type);
			}

			arrIndex = evalExpression();

			//make sure array index is an integer
			if(!typecheckMemberAccessExpression(arrIndex->evalType)) {
				uint32_t startLine = varToken->line+1;
				uint32_t endLine = arrIndex->endLine+1;
				throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "array indexing", arrIndex->evalType); 
			}

			if(peek()->type != RIGHT_BRACKET) {
				uint32_t startLine = varToken->line+1;
				uint32_t endLine = arrIndex->endLine+1;
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Array indexing operation must end with ']'");
			}

			Token* rightBracketToken = consume();
			initValue = new ArrayAccessNode(new VariableNode(variable, symbolTable, varToken->line), arrIndex, -1);
		}

		//variable must be assigned (can't just be declared)
    Token* assignmentToken = consume();
    if(!isAssignmentOperatorTokenType(assignmentToken->type)) {

      uint32_t startLine = varToken->line+1;
			uint32_t endLine = (assignmentToken->type == END) ? arrIndex->endLine+1 : assignmentToken->line+1;

			if(assignmentToken->type == END) {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected '=' followed by assignment value");
			} else {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), assignmentToken->lexeme, "Expected '=' followed by assignment value");
			}
    }
    
    AbstractExpressionNode* expression = evalExpression();
		
		//if not an array type, set the initial value
		if(initValue == NULL) {
			initValue = new VariableNode(variable, symbolTable, varToken->line);
		}

    switch(assignmentToken->type) {
      case EQ: break;
      case ADD_EQ: expression = new ArithmeticOperatorNode(ADD_OP, initValue, expression); break; 
      case SUBTRACT_EQ: expression = new ArithmeticOperatorNode(SUBTRACT_OP, initValue, expression); break;
      case EXPONENT_EQ: expression = new ArithmeticOperatorNode(EXPONENT_OP, initValue, expression); break;
      case MULTIPLY_EQ: expression = new ArithmeticOperatorNode(MULTIPLY_OP, initValue, expression); break;
      case DIVIDE_EQ: expression = new ArithmeticOperatorNode(DIVIDE_OP, initValue, expression); break;
      case AND_EQ: {
        if(type == BOOL_T)
          expression = new BitLogicalOperatorNode(AND_OP, initValue, expression);
        else 
          expression = new BitLogicalOperatorNode(BIT_AND_OP, initValue, expression);
        break;
      }
      
      case XOR_EQ: {
        if(type == BOOL_T)
          expression = new BitLogicalOperatorNode(XOR_OP, initValue, expression);
        else 
          expression = new BitLogicalOperatorNode(BIT_XOR_OP, initValue, expression);
        break;
      }
      
      case OR_EQ: {
        if(type == BOOL_T)
          expression = new BitLogicalOperatorNode(OR_OP, initValue, expression);
        else 
          expression = new BitLogicalOperatorNode(BIT_OR_OP, initValue, expression);
        break;
      }
    }
    
		//if array assignment, just deal with it here for now
		if(type == ARRAY_T) {
			return new ArrayAssignmentStatementNode(variable, arrIndex, expression, symbolTable);
		}

    //make sure implicit cast is valid
    if(!typecheckImplicitCastExpression(expression->evalType, type)) {
      throw StaticCastError(varToken->line+1, expression->endLine+1, getCodeLineBlock(varToken->line, expression->endLine), expression->evalType, type, false);
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
      
			Token* leftBraceToken = consume();
      vector<AbstractStatementNode*>* statements = new vector<AbstractStatementNode*>();
      
      //enter a new scope in symbol table (for static scope-checking)
      symbolTable->enterNewScope();
			AbstractStatementNode* currentStatement = NULL;

      while(peek()->type != RIGHT_BRACE) {

				//make sure closing brace is found before code ends
				if(peek()->type == END) {
					//TODO
				}

				currentStatement = addStatement();
        statements->push_back(currentStatement);  
      }
      
      consume(); //consume right brace
      
      //leave scope
      symbolTable->leaveScope();
      
      return new GroupedStatementNode(statements, symbolTable);
    }
    
    case WHILE: {
    
      Token* whileToken = consume(); //consume while Token
      AbstractExpressionNode* condition = evalExpression();
      
			//make sure the conditional expression is a boolean
      if(condition->evalType != BOOL_T) {

				uint32_t startLine = whileToken->line+1;
				uint32_t endLine = condition->endLine+1;

				string message = "Expected bool expression for while condition, not a ";
				message.append(toStringParseDataType(condition->evalType));
				message.append(" one");

        throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), copyString(message.c_str()));
      }
      
      AbstractStatementNode* body = addStatement();
      return new WhileStatementNode(condition, body, symbolTable);
    }
    
    case FOR: {
      
      Token* forToken = consume(); //consume for Token
      Token* leftParenToken = consume();


			//make sure the syntax is exactly correct
      if(leftParenToken->type != LEFT_PAREN) {

				uint32_t startLine = forToken->line+1;
				uint32_t endLine = (leftParenToken->type == END) ? startLine : leftParenToken->line+1;

				if(leftParenToken->type == END) {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected '(' following 'for' declaration");
				} else {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), leftParenToken->lexeme, "Expected '(' following 'for' declaration");
				}
      }
      
      AbstractStatementNode* initialization = addStatement();
      
      if(peek()->type != SEMICOLON) {
        std::cout << "ERROR: ; expected in 'for' construct" << std::endl;
        return NULL;
      } else {
        consume();
      }
      
      AbstractExpressionNode* condition = evalExpression();
      
      if(condition->evalType != BOOL_T) {
        std::cout << "ERROR: expression must evaluate to a bool" << std::endl;
        return NULL;
      }
      
      if(peek()->type != SEMICOLON) {
        std::cout << "ERROR: ; expected in 'for' construct" << std::endl;
        return NULL;
      } else {
        consume();
      }
      
      AbstractStatementNode* update = addStatement();
      
      if(peek()->type != RIGHT_PAREN) {
        std::cout << "ERROR: ) must follow here in 'for' construct" << std::endl;
        return NULL;
      } else {
        consume();
      }
      
      AbstractStatementNode* body = addStatement();
      return new ForStatementNode(initialization, update, body, condition, symbolTable);
    }
    
    case IF: {
    
      vector<AbstractExpressionNode*>* cond = new vector<AbstractExpressionNode*>();
      vector<AbstractStatementNode*>* stat = new vector<AbstractStatementNode*>();
      
      //consume the IF token and add first condition-statement pair
      Token* ifToken = consume();
      AbstractExpressionNode* e = evalExpression();
      
			//make sure the condition is a boolean
      if(e->evalType != BOOL_T) {
        
				uint32_t startLine = ifToken->line+1;
				uint32_t endLine = e->endLine+1;

				string message = "'if' condition requires bool expression, not a ";
				message.append(toStringParseDataType(e->evalType));
				message.append(" one");

				throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), copyString(message.c_str()));

      } else {
        cond->push_back(e);
        stat->push_back(addStatement());
      }
      
      //now append to the chain as needed
      while(peek()->type == ELIF || peek()->type == ELSE) {
        
        if(peek()->type == ELIF) {
          
          //consume ELIF token and add condition-statement pair
          Token* elifToken = consume();
          AbstractExpressionNode* exp = evalExpression();
          
					//make sure the condition is a boolean
          if(exp->evalType != BOOL_T) {

						uint32_t startLine = elifToken->line+1;
						uint32_t endLine = exp->endLine+1;

						string message = "'elif' condition requires a bool expression, not a ";
						message.append(toStringParseDataType(exp->evalType));
						message.append(" one");

            throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), copyString(message.c_str()));

          } else {
            cond->push_back(exp);
            stat->push_back(addStatement());
          }
           
        } else {
          
          //consume ELSE token and add dummy true-statement pair
          Token* elseToken = consume();
          
          ParseData d;
          d.type = BOOL_T; d.value.integer = true;
          LiteralNode* exp = new LiteralNode(d, elseToken->line);
          
          cond->push_back(exp);
          stat->push_back(addStatement());
          
          return new ConditionalStatementNode(cond, stat, symbolTable);
        }  
      }
      
      return new ConditionalStatementNode(cond, stat, symbolTable);
    }

		case RETURN: {
			consume(); //consume return
			return new ReturnStatementNode(evalExpression(), returnFlag.back(), returnValue.back());
		}

		case FUN: {

			Token* funToken = consume(); //consume the FUN token
			Token* varToken = consume(); //function name

			//next token should be a variable, the function name
			if(varToken->type != VARIABLE) {

				uint32_t startLine = funToken->line+1;
				uint32_t endLine = (varToken->type == END) ? startLine : varToken->line+1;

				if(varToken->type == END) {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "expected function name after 'fun' declaration");
				} else {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), varToken->lexeme, "expected function name after 'fun' declaration");
				} 
			}

			string functionName(varToken->lexeme);

			//now create a Function struct
			Function* function = (Function*) malloc(sizeof(Function));
			function->returnFlag = (bool*) malloc(sizeof(bool));
			function->returnValue = (ParseData*) malloc(sizeof(ParseData)); 

			//assign the global returnFlag and returnValue pointers to this function
			returnFlag.push_back(function->returnFlag);
			returnValue.push_back(function->returnValue);

			//now read in arguments enclosed in parentheses
			Token* leftParenToken = consume(); //consume (

			//if the next token is not a parenthesis, throw an error
			if(leftParenToken->type != LEFT_PAREN) {

				uint32_t startLine = funToken->line+1;
				uint32_t endLine = (leftParenToken->type == END) ? varToken->line+1 : leftParenToken->line+1;

				if(leftParenToken->type == END) {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected '(' to begin function parameter list"); 
				} else {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), leftParenToken->lexeme, "Expected '(' to begin function parameter list");
				}
			}
			
			//first scan to figure out how many arguments must be allocated in Function
			uint32_t currentIndex = 0;
			uint32_t argCount = 0;

			while(peekAhead(currentIndex)->type != RIGHT_PAREN) {

				//make sure you have a type then variable
				if(!isTypeTokenType(peekAhead(currentIndex)->type)) {
					cout << "ERROR: expected argument type" << endl;
					return NULL;	
				}
				currentIndex++;

				if(peekAhead(currentIndex)->type != VARIABLE) {
					cout << "ERROR: expected argument variable name" << endl;
					return NULL;
				}
				currentIndex++;

				argCount++;

				//if next token is not closing parenthesis, it must be a comma
				TokenType nextToken = peekAhead(currentIndex)->type;
				if(nextToken != RIGHT_PAREN && nextToken != COMMA) {
					cout << "ERROR: expected comma separating arguments or closing parenthesis" << endl;
					return NULL;
				} else if(nextToken == COMMA) {
					currentIndex++; //"consume" COMMA token
				}
			}

			//now allocate memory for argument names and types
			function->numArgs = argCount;
			ParseDataType* argTypes = new ParseDataType[argCount];
			char** argNames = (char**) malloc(sizeof(char*) * argCount);

			//now read in arguments for real
			uint32_t argIndex = 0;
			while(peek()->type != RIGHT_PAREN) {

				//read in parameter type
				argTypes[argIndex] = typeTokenConversion(consume()->type);

				//read in parameter name
				const char* aName = consume()->lexeme;
				argNames[argIndex] = copyString(aName);

				argIndex++;
				if(argIndex < argCount)
					consume(); //consume ,
			}
			consume(); //consume ')' token

			function->argTypes = argTypes;
			function->argNames = argNames;

			//now get the return type
			if(peek()->type != RIGHTARROW) {
				cout << "ERROR: expected -> followed by a return type" << endl;
				return NULL;
			}
			consume(); //consume ->

			function->returnType = typeTokenConversion(consume()->type);

			//just declare function up front with dummy data and Function public interface data
			//(to allow for recursive definitions)
			ParseData dummyFunctionData;
			dummyFunctionData.type = FUN_T;
			dummyFunctionData.value.allocated = function;
			symbolTable->declare(functionName, dummyFunctionData);


			//now declare the variables just for scope-checking purposes
			//and enter a new scope for function body
      symbolTable->enterNewScope();
			NewAssignmentStatementNode* dummy;

			for(uint32_t i = 0; i < argCount; i++) {
				//this construction declares variable as well
				string argName(argNames[i]);
				dummy = new NewAssignmentStatementNode(argName, argTypes[i], symbolTable);
			}
			
			//now store AbstractStatementNode* vector representing function body
			if(peek()->type != LEFT_BRACE) {
				cout << "ERROR: expected braced function body" << endl;
				return NULL;
			}
			consume(); //consume {

			//represents statements in body
      vector<AbstractStatementNode*>* body = new vector<AbstractStatementNode*>();
      
      while(peek()->type != RIGHT_BRACE) {
        body->push_back(addStatement());  
      }
      consume(); //consume }
      
      //leave scope
      symbolTable->leaveScope();
			
			function->body = body;

			//reset global pointers
			returnFlag.pop_back();
			returnValue.pop_back();

			//now return a FunctionStatementNode
			return new FunctionStatementNode(functionName, function, symbolTable);
		}
    
    default: return new ExpressionStatementNode(evalExpression(), symbolTable);
  }
  
}

//generate Abstract Syntax Tree from list of tokens
vector<AbstractStatementNode*>* parse(vector<Token>* tokenRef, vector<char*>* sourceCodeLines) {

  codeLines = sourceCodeLines;

  //set static variables to correct initial values
  tokenIndex = 0;
  tokens = tokenRef;
  symbolTable = new SymbolTable();
	returnFlag = vector<bool*>();
	returnValue = vector<ParseData*>();
  
  //create empty statement vector
  vector<AbstractStatementNode*>* statements = new vector<AbstractStatementNode*>();
  
  //append statement nodes until END is reached
  while(peek()->type != END) {
    statements->push_back(addStatement());
  }
  
  return statements;
}


