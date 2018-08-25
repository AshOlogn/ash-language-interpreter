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

//used by return statements to place value in correct memory location
//and keep track of function type
static vector<bool*> returnFlag;
static vector<ParseData*> returnValue;
static vector<ParseDataType> returnType;

//some state variables
static bool insideClassDefinition;

/////////////////////////////////
//////   Utility Functions  /////
/////////////////////////////////

//return current Token and advance
Token* consume() {
  return (Token*) &(tokens->at(tokenIndex++));
}

void stepBack() {
	tokenIndex--;
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

//used to parse function definition and return struct with information
Function* parseFunction(uint32_t startLine, uint32_t secondStartLine, string functionName) {

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

		uint32_t endLine = (leftParenToken->type == END) ? secondStartLine : leftParenToken->line+1;

		if(leftParenToken->type == END) {
			throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected '(' to begin function parameter list"); 
		} else {
			throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), leftParenToken->lexeme, "Expected '(' to begin function parameter list");
		}
	}

	//first scan to figure out how many arguments must be allocated in Function
	uint32_t currentIndex = 0;
	uint32_t argCount = 0;

	uint32_t currentErrorLine = leftParenToken->line+1;
	while(peekAhead(currentIndex)->type != RIGHT_PAREN) {

		//make sure you have a type then variable
		if(!isTypeTokenType(peekAhead(currentIndex)->type)) {

			Token* tempTypeToken = peekAhead(currentIndex);	
			uint32_t endLine = (tempTypeToken->type == END) ? currentErrorLine : tempTypeToken->line+1;

			if(tempTypeToken->type == END) {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected function parameter type");
			} else {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), tempTypeToken->lexeme, "Expected function parameter type");
			}
		}
		currentErrorLine = peekAhead(currentIndex)->line+1;
		currentIndex++;

		//could be an array type
		if(peekAhead(currentIndex)->type == LEFT_BRACKET) {

			currentErrorLine = peekAhead(currentIndex)->line+1;
			currentIndex++;

			//next token must be ']'
			if(peekAhead(currentIndex)->type != RIGHT_BRACKET) {

				uint32_t endLine = (peekAhead(currentIndex)->type == END) ? currentErrorLine : peekAhead(currentIndex)->line+1;

				if(peekAhead(currentIndex)->type == END) {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected closing ']' in array type");		
				} else {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), peekAhead(currentIndex)->lexeme, "Expected closing ']' in array type");						
				}
			}

			currentErrorLine = peekAhead(currentIndex)->line+1;
			currentIndex++;
		}

		//make sure parameter name follows type
		if(peekAhead(currentIndex)->type != VARIABLE) {

			Token* tempVariableToken = peekAhead(currentIndex);
			uint32_t endLine = (tempVariableToken->type == END) ? currentErrorLine : tempVariableToken->line+1;

			if(tempVariableToken->type == END) {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected function parameter name");
			} else {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), tempVariableToken->lexeme, "Expected function parameter name");
			}
		}
		currentErrorLine = peekAhead(currentIndex)->line+1;
		currentIndex++;
		argCount++;

		//if next token is not closing parenthesis, it must be a comma
		Token* nextToken = peekAhead(currentIndex);				
		if(nextToken->type != RIGHT_PAREN && nextToken->type != COMMA) {

			uint32_t endLine = (nextToken->type == END) ? currentErrorLine : nextToken->line+1;

			if(nextToken->type == END) {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected ',' separating arguments or ')' terminating argument list");
			} else {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), nextToken->lexeme, "Expected ',' separating arguments or ')' terminating argument list"); 
			} 

		} else if(nextToken->type == COMMA) {
			currentErrorLine = nextToken->line+1;
			currentIndex++; //"consume" COMMA token
		}
	}

	//now allocate memory for argument names and types
	function->numArgs = argCount;
	ParseDataType* argTypes = new ParseDataType[argCount];
	ParseDataType* argSubTypes = new ParseDataType[argCount];
	char** argNames = (char**) malloc(sizeof(char*) * argCount);

	//keep track of variable line numbers
	uint32_t typeLines[argCount];
	uint32_t varLines[argCount];

	//now read in arguments for real
	uint32_t argIndex = 0;
	while(peek()->type != RIGHT_PAREN) {

		//read in parameter type
		Token* currTypeToken = consume();

		//could be an array type
		if(peek()->type == LEFT_BRACKET) {

			argTypes[argIndex] = ARRAY_T;
			argSubTypes[argIndex] = typeTokenConversion(currTypeToken->type);

			consume(); //consume '['
			typeLines[argIndex] = consume()->line+1;

		} else {

			argTypes[argIndex] = typeTokenConversion(currTypeToken->type);
			argSubTypes[argIndex] = INVALID_T;
			typeLines[argIndex] = currTypeToken->line+1;
		}

		//read in parameter name
		Token* currVarToken = consume();
		const char* aName = currVarToken->lexeme;
		argNames[argIndex] = copyString(aName);
		varLines[argIndex] = currVarToken->line+1;

		argIndex++;
		if(argIndex < argCount)
			consume(); //consume ,
	}
	leftParenToken = consume(); //consume ')' token

	function->argTypes = argTypes;
	function->argSubTypes = argSubTypes;
	function->argNames = argNames;

	//now get the return type
	Token* rightArrowToken = consume(); //consume ->
	if(rightArrowToken->type != RIGHTARROW) {
		
		uint32_t endLine = (rightArrowToken->line == END) ? leftParenToken->line+1 : rightArrowToken->line+1;

		if(rightArrowToken->line == END) {
			throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected -> followed by return type");
		} else {
			throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), rightArrowToken->lexeme, "Expected -> followed by return type");
		}
	}

	//make sure following token is a return type
	Token* returnTypeToken = consume();
	if(!isTypeTokenType(returnTypeToken->type)) {

		uint32_t endLine = (returnTypeToken->type == END) ? rightArrowToken->line+1 : returnTypeToken->line+1;

		if(returnTypeToken->type == END) {
			throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected function return type"); 
		} else {
			throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), returnTypeToken->lexeme, "Expected function return type"); 
		}
	}

	returnType.push_back(typeTokenConversion(returnTypeToken->type));
	function->returnType = typeTokenConversion(returnTypeToken->type);

	//just declare function up front with dummy data and Function public interface data
	//(to allow for recursive definitions)
	ParseData dummyFunctionData;
	dummyFunctionData.type = FUN_T;
	dummyFunctionData.value.allocated = function;
	symbolTable->declare(functionName, dummyFunctionData);

	//now declare the variables just for scope-checking purposes
	//and enter a new scope (in both tables) for function body
	symbolTable->enterNewScope();
	NewAssignmentStatementNode* dummy;

	for(uint32_t i = 0; i < argCount; i++) {
		//this construction declares variable as well
		string argName(argNames[i]);
		dummy = new NewAssignmentStatementNode(argName, argTypes[i], symbolTable, typeLines[i], varLines[i]);
	}

	//now store AbstractStatementNode* vector representing function body
	//make sure the body is braced
	Token* leftBraceToken = consume(); //consume {
	if(leftBraceToken->type != LEFT_BRACE) {

		uint32_t endLine = (leftBraceToken->type == END) ? returnTypeToken->line+1 : leftBraceToken->line+1;

		if(leftBraceToken->type == END) {
			throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected '{' followed by function body");
		} else {
			throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), leftBraceToken->lexeme, "Expected '{' followed by function body");
		}
	}

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
	returnType.pop_back();

	return function;
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

		//throw error if ')' is not found
    if(peek()->type != RIGHT_PAREN) {
      throw ParseSyntaxError(startGroupToken->line+1, codeLines->at(startGroupToken->line), "(", "The parenthesized expression is missing a ')'");
    } else {
      endGroupToken = consume(); //consume right parenthesis
    }

    return new GroupedExpressionNode(next, startGroupToken->line+1, endGroupToken->line+1);

	} else if(peek()->type == LEFT_BRACKET) {

		//initialized array
		Token* leftBracketToken = consume();

		//insert the initial values into a list
		vector<AbstractExpressionNode*> initValues;

		//keep track of last line for error-handling purposes
		uint32_t currEndLine = leftBracketToken->line+1;

		while(peek()->type != RIGHT_BRACKET) {

			//if code ends without a closing ']', throw an error
			if(peek()->type == END) {
				throw ParseSyntaxError(leftBracketToken->line+1, currEndLine, getCodeLineBlock(leftBracketToken->line, currEndLine-1), "Expected ']' to terminate array initializer list");
			}

			//add element to the array and update end line
			AbstractExpressionNode* arrayElement = evalExpression();
			currEndLine = arrayElement->endLine;
			initValues.push_back(arrayElement);

			if(peek()->type == COMMA) {
				//consume , token and update end line
				Token* commaToken = consume(); 
				currEndLine = commaToken->line+1;
			} else if(peek()->type == RIGHT_BRACKET) {
				//do nothing, deal with it outside the while loop
			} else {
				//throw error if initializer list is not terminated
				throw ParseSyntaxError(leftBracketToken->line+1, peek()->line+1, getCodeLineBlock(leftBracketToken->line, peek()->line), 
																	"Expected ']' to terminate array initializer list");
			}
		}

		Token* rightBracketToken = consume(); //consume ']'

		uint32_t length = (uint32_t) initValues.size();
		AbstractExpressionNode** values = (AbstractExpressionNode**) malloc(sizeof(AbstractExpressionNode*) * length);

		//determine overall type of initialized array
		ParseDataType arrayType = arrayListType(&initValues);

		//copy references out of list into array, casting to overall type
		for(uint32_t i = 0; i < length; i++) {
			values[i] = new CastNode(initValues[i], arrayType, initValues[i]->startLine);
		}

		//convert fixed length into a LiteralNode
		ParseData len;
		len.type = UINT32_T;
		len.value.integer = length;

		return new ArrayNode(arrayType, true, new LiteralNode(len, -1), values, leftBracketToken->line+1, rightBracketToken->line+1);

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
			throw ParseSyntaxError(newToken->line+1, length->endLine+1, getCodeLineBlock(newToken->line, length->endLine), "Expected ']' as part of array instantiation");
		}

		Token* rightBracketToken = consume();

		//now return ArrayNode with appropriate information
		return new ArrayNode(subtype, false, length, NULL, newToken->line, rightBracketToken->line);

  } else if(peek()->type == VARIABLE) {

		Token* variable = consume();

		//make sure variable is already declared
		string variableName((char*) variable->lexeme);
		if(!symbolTable->isDeclared(variableName)) {
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
			ParseDataType* argSubTypes = function->argSubTypes;

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
				if(argTypes[i] == ARRAY_T && arguments[i]->evalType == ARRAY_T) {

					//both args are arrays, so check member types
					if(!typecheckImplicitCastExpression(arguments[i]->subType, argSubTypes[i])) {
						
						uint32_t startLine = variable->line+1;
						uint32_t endLine = arguments[i]->endLine+1;

						string message = "Cannot implicitly convert a ";
						message.append(toStringParseDataType(arguments[i]->subType));
						message.append(" array argument to ");
						message.append(toStringParseDataType(argSubTypes[i]));
						message.append(" array type");

						throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), copyString(message.c_str()));
					}

				} else {

					//regular type check
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
			return new VariableNode(variableName, symbolTable, variable->line+1);
		}
  
  } else {

    Token* literal = consume();
    ParseData p = {typeTokenConversion(literal->type), literal->value};
    return new LiteralNode(p, literal->line+1);
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
      
      return new ArrayAccessNode(head, new LiteralNode(startIndex, leftBracketToken->line+1), 
                                  new LiteralNode(endIndex, rightBracketToken->line+1), getCodeLineBlock(head->startLine-1, rightBracketToken->line), rightBracketToken->line+1);

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
        return new ArrayAccessNode(head, new LiteralNode(startIndex, leftBracketToken->line+1), end, getCodeLineBlock(head->startLine-1, rightBracketToken->line), rightBracketToken->line+1);
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
    
        return new ArrayAccessNode(head, start, new LiteralNode(endIndex, rightBracketToken->line+1), getCodeLineBlock(head->startLine-1, rightBracketToken->line), rightBracketToken->line+1);

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
          return new ArrayAccessNode(head, start, end, getCodeLineBlock(head->startLine-1, rightBracketToken->line), rightBracketToken->line+1);
        }
      }

    } else {
      
      //accessing single value
      if(peek()->type != RIGHT_BRACKET) {
        
        //!!! If [ does not have a corresponding ], throw an error
        throw ParseSyntaxError(leftBracketToken->line+1, codeLines->at(leftBracketToken->line), "[", "Array access operation must end with ']'");

      } else {
        Token* rightBracketToken = consume(); //consume ]
        return new ArrayAccessNode(head, start, getCodeLineBlock(head->startLine-1, rightBracketToken->line), rightBracketToken->line+1);
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
      throw StaticVariableScopeError(varNode->startLine, varNode->variable, getCodeLineBlock(varNode->startLine-1, varNode->startLine-1), false);   
    }

    //get variable type
    ParseDataType varType = varNode->evalType;

    Token* firstPlus = consume(); //consume first + or -
    Token* secondPlus = consume(); //consume second + or -

    if(secondPlus->type == ADD) {
      //make sure type is correct
      if(!typecheckUnaryExpression(POSTFIX_INC_OP, varType)) {
        throw StaticTypeError(varNode->startLine, secondPlus->line+1, getCodeLineBlock(varNode->startLine-1, secondPlus->line), "postfix increment", varType);
      }
      return new UnaryOperatorNode(POSTFIX_INC_OP, varNode, symbolTable, secondPlus->line+1);

    } else {
      //make sure type is correct
      if(!typecheckUnaryExpression(POSTFIX_DEC_OP, varType)) {
        throw StaticTypeError(varNode->startLine, secondPlus->line+1, getCodeLineBlock(varNode->startLine-1, secondPlus->line), "postfix decrement", varType);
      }
      return new UnaryOperatorNode(POSTFIX_DEC_OP, varNode, symbolTable, secondPlus->line+1); 
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
      throw ParseSyntaxError(firstPlus->line+1, head->endLine, getCodeLineBlock(firstPlus->line, head->endLine-1), "Prefix operator expects variable argument");
    }

    ParseDataType type = varNode->evalType;
    ParseOperatorType op = (firstPlus->type == ADD) ? PREFIX_INC_OP : PREFIX_DEC_OP;

    //if it's not a number, throw a type error
    if(!typecheckUnaryExpression(op, type)) {
      throw StaticTypeError(firstPlus->line+1, head->endLine, getCodeLineBlock(firstPlus->line, head->endLine-1), (op == PREFIX_INC_OP) ? "prefix increment" : "prefix decrement", type);
    }

    return new UnaryOperatorNode(op, varNode, symbolTable, firstPlus->line+1);

  } else if(isSignNotTokenType(peek()->type)) {

    Token* signNotToken = consume();
    ParseOperatorType op = unaryTokenConversion(signNotToken->type);
    AbstractExpressionNode* next = evalPrefixCastSignNot();

    if(typecheckUnaryExpression(op, next->evalType)) {
      return new UnaryOperatorNode(op, next, signNotToken->line+1);
    } else {
      //if type mismatch, throw error
      throw StaticTypeError(signNotToken->line+1, next->endLine+1, getCodeLineBlock(signNotToken->line, next->endLine), toWordParseOperatorType(op), next->evalType);
    }

  } else if(isCastStructure()) {   
    
    Token* leftParenToken = consume(); //consume (
		Token* typeToken = consume();
    ParseDataType finalType = typeTokenConversion(typeToken->type);
    Token* rightParenToken = consume(); //consume ')'

		//if there is no closing ')', throw error
		if(rightParenToken->type != RIGHT_PAREN) {

			uint32_t startLine = leftParenToken->line+1;
			uint32_t endLine = (rightParenToken->type == END) ? typeToken->line+1 : rightParenToken->line+1;

			if(rightParenToken->type == END) {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected ')' to end cast operation");
			} else {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), rightParenToken->lexeme, "Expected ')' to end cast operation");
			}
		}
    
    AbstractExpressionNode* next = evalPrefixCastSignNot();
    if(typecheckExplicitCastExpression(next->evalType, finalType)) {
      return new CastNode(next, finalType, leftParenToken->line+1);
    } else {
      throw StaticCastError(leftParenToken->line+1, next->endLine, getCodeLineBlock(leftParenToken->line, next->endLine-1), next->evalType, finalType, true);
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), "exponentiation", head->evalType, next->evalType);
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), toWordParseOperatorType(op), head->evalType, next->evalType);
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), toWordParseOperatorType(op), head->evalType, next->evalType);
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), toWordParseOperatorType(op), head->evalType, next->evalType);
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), toWordParseOperatorType(op), head->evalType, next->evalType); 
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), toWordParseOperatorType(op), head->evalType, next->evalType);  
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), "bitwise AND", head->evalType, next->evalType); 
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), "bitwise XOR", head->evalType, next->evalType); 
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), "bitwise OR", head->evalType, next->evalType);
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), "logical AND", head->evalType, next->evalType);  
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), "logical XOR", head->evalType, next->evalType); 
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
      throw StaticTypeError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), "logical OR", head->evalType, next->evalType);  
    }
  }
 
  return head;
}


//  = += -= *= **= /= &= ^= |= <<= >>=
AbstractExpressionNode* evalAssignment() {
  
  AbstractExpressionNode* head = evalLogicOr();
  VariableNode* var = dynamic_cast<VariableNode*>(head);
	ArrayAccessNode* arrVar = dynamic_cast<ArrayAccessNode*>(head);
  
  if((var || arrVar) && isAssignmentOperatorTokenType(peek()->type)) {
  
    //if just variable, make sure it is already declared in some scope
		if(var && !symbolTable->isDeclared(var->variable)) {
      throw StaticVariableScopeError(var->startLine, var->variable, getCodeLineBlock(var->startLine-1, var->startLine-1), false);
		}
    
    //get variable's type
    ParseDataType type = head->evalType;
    
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
					uint32_t startLine = head->startLine;
					throw StaticTypeError(startLine, next->endLine, getCodeLineBlock(startLine-1, next->endLine-1), 
																toStringParseOperatorType(opType), type, next->evalType); 
				}

			} else {

				if(!typecheckBitLogicalExpression(opType, type, next->evalType)) {
					uint32_t startLine = head->startLine;
					throw StaticTypeError(startLine, next->endLine, getCodeLineBlock(startLine-1, next->endLine-1), 
																toStringParseOperatorType(opType), type, next->evalType); 
				}
			}
		}
    
    switch(assignmentToken->type) {
      case EQ: break;
      case ADD_EQ: next = new ArithmeticOperatorNode(ADD_OP, head, next); break; 
      case SUBTRACT_EQ: next = new ArithmeticOperatorNode(SUBTRACT_OP, head, next); break;
      case EXPONENT_EQ: next = new ArithmeticOperatorNode(EXPONENT_OP, head, next); break;
      case MULTIPLY_EQ: next = new ArithmeticOperatorNode(MULTIPLY_OP, head, next); break;
      case DIVIDE_EQ: next = new ArithmeticOperatorNode(DIVIDE_OP, head, next); break;
      case AND_EQ: {
        if(type == BOOL_T)
          next = new BitLogicalOperatorNode(AND_OP, head, next);
        else 
          next = new BitLogicalOperatorNode(BIT_AND_OP, head, next);
        break;
      }
      
      case XOR_EQ: {
        if(type == BOOL_T)
          next = new BitLogicalOperatorNode(XOR_OP, head, next);
        else 
          next = new BitLogicalOperatorNode(BIT_XOR_OP, head, next);
        break;
      }
      
      case OR_EQ: {
        if(type == BOOL_T)
          next = new BitLogicalOperatorNode(OR_OP, head, next);
        else 
          next = new BitLogicalOperatorNode(BIT_OR_OP, head, next);
        break;
      }
    }
    
    //now make sure that implicit type cast is valid
    if(!typecheckImplicitCastExpression(next->evalType, type)) {
      throw StaticCastError(head->startLine, next->endLine, getCodeLineBlock(head->startLine-1, next->endLine-1), next->evalType, type, false);
    }
    
		if(var) {
			return new AssignmentExpressionNode(var->variable, type, next, symbolTable, var->startLine);
		} else {
			return new ArrayAssignmentExpressionNode(arrVar->array, arrVar->start, next, getCodeLineBlock(arrVar->startLine-1, next->endLine-1), symbolTable);
		}
    
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

		//only in the case of an array or string
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
				return new NewAssignmentStatementNode(variable, type, subtype, expression, symbolTable, typeToken->line+1);
			}

			//non-array case
      if(!typecheckImplicitCastExpression(expression->evalType, type)) {
        throw StaticCastError(variableToken->line+1, expression->endLine, getCodeLineBlock(variableToken->line, expression->endLine-1), expression->evalType, type, false); 
      } else {
        return new NewAssignmentStatementNode(variable, type, expression, symbolTable, typeToken->line+1);
      }
      
    } else {  

      //no initial value
			if(type == ARRAY_T) {
				return new NewAssignmentStatementNode(variable, type, subtype, symbolTable, typeToken->line+1, variableToken->line+1);
			} else {
				return new NewAssignmentStatementNode(variable, type, symbolTable, typeToken->line+1, variableToken->line+1);
			}
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
    
		//consider the ++ and -- cases
		if(isPostfixStructure()) {
			stepBack();
			return new ExpressionStatementNode(evalPostfixMemberAccess(), symbolTable);
		}

		//if it is a function call, treat it like an "expression" statement
		if(peek()->type == LEFT_PAREN) {
			stepBack();
			return new ExpressionStatementNode(evalExpression(), symbolTable);
		}

    //get variable type (and maybe array/string element subtype) from the symbol table
		ParseDataType type;
		ParseDataType subtype = INVALID_T;
		bool isIndexAssignment = false;

    type = (symbolTable->get(variable)).type;
		if(type == ARRAY_T) {
			subtype = ((Array*) (symbolTable->get(variable)).value.allocated)->subtype;
		} else if(type == STRING_T) {
			subtype = CHAR_T;
		}

		//initial value of variable being assigned to (in event of *=, -=, etc.)
		AbstractExpressionNode* initValue = NULL;

		//deal with array/string index assignment possibility
		AbstractExpressionNode* arrIndex;
		if(peek()->type == LEFT_BRACKET) {

			isIndexAssignment = true;
			Token* leftBracketToken = consume();

			//make sure it is an array
			if(type != ARRAY_T && type != STRING_T) {
				uint32_t startLine = varToken->line+1;
				uint32_t endLine = leftBracketToken->line+1;
				throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "array index assignment", type);
			}

			arrIndex = evalExpression();

			//make sure array index is an integer
			if(!typecheckMemberAccessExpression(arrIndex->evalType)) {
				uint32_t startLine = varToken->line+1;
				uint32_t endLine = arrIndex->endLine;
				throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "array indexing", arrIndex->evalType); 
			}

			if(peek()->type != RIGHT_BRACKET) {
				uint32_t startLine = varToken->line+1;
				uint32_t endLine = arrIndex->endLine;
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Array indexing operation must end with ']'");
			}

			Token* rightBracketToken = consume();
			initValue = new ArrayAccessNode(new VariableNode(variable, symbolTable, varToken->line+1), arrIndex, getCodeLineBlock(varToken->line, rightBracketToken->line), rightBracketToken->line+1);
		}

		//variable must be assigned (can't just be declared)
    Token* assignmentToken = consume();
    if(!isAssignmentOperatorTokenType(assignmentToken->type)) {

      uint32_t startLine = varToken->line+1;
			uint32_t endLine = (assignmentToken->type == END) ? arrIndex->endLine : assignmentToken->line+1;

			if(assignmentToken->type == END) {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected '=' followed by assignment value");
			} else {
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), assignmentToken->lexeme, "Expected '=' followed by assignment value");
			}
    }
    
    AbstractExpressionNode* expression = evalExpression();
		
		//if not an array type, set the initial value
		if(initValue == NULL) {
			initValue = new VariableNode(variable, symbolTable, varToken->line+1);
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
		if(isIndexAssignment) {

			//make sure implicit cast is valid
			if(!typecheckImplicitCastExpression(expression->evalType, subtype)) {
				throw StaticCastError(varToken->line+1, expression->endLine, getCodeLineBlock(varToken->line, expression->endLine-1), expression->evalType, type, false);
			}

			return new ArrayAssignmentStatementNode(variable, type == ARRAY_T, arrIndex, expression, symbolTable, getCodeLineBlock(varToken->line, expression->endLine-1), varToken->line+1);
		}

    //make sure implicit cast is valid
    if(!typecheckImplicitCastExpression(expression->evalType, type)) {
      throw StaticCastError(varToken->line+1, expression->endLine, getCodeLineBlock(varToken->line, expression->endLine-1), expression->evalType, type, false);
    } else {
      return new AssignmentStatementNode(variable, expression, symbolTable, varToken->line+1);
    }
  }
	
  switch(t->type) {
    
    case PRINTLN: {
      consume();
      return new PrintLineStatementNode(evalExpression(), symbolTable, t->line+1);
    }
    
    case PRINT: {  
      consume();
      return new PrintStatementNode(evalExpression(), symbolTable, t->line+1);
    }
    
    case LEFT_BRACE: {
      
			Token* leftBraceToken = consume();
      vector<AbstractStatementNode*>* statements = new vector<AbstractStatementNode*>();
      
      //enter a new scope in both symbol tables (for static scope-checking)
      symbolTable->enterNewScope();
			AbstractStatementNode* currentStatement = NULL;

			uint32_t currentEndLine = leftBraceToken->line+1;
      while(peek()->type != RIGHT_BRACE) {

				//make sure closing brace is found before code ends
				if(peek()->type == END) {
					uint32_t startLine = leftBraceToken->line+1;
					uint32_t endLine = currentEndLine;

					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected closing '}' for block statement");
				}

				currentStatement = addStatement();
				currentEndLine = currentStatement->endLine;
        statements->push_back(currentStatement);  
      }
      
      Token* rightBraceToken = consume(); //consume right brace
      
      //leave scope
      symbolTable->leaveScope();

      return new GroupedStatementNode(statements, symbolTable, leftBraceToken->line+1, rightBraceToken->line+1);
    }
    
    case WHILE: {
    
      Token* whileToken = consume(); //consume while Token
      AbstractExpressionNode* condition = evalExpression();
      
			//make sure the conditional expression is a boolean
      if(condition->evalType != BOOL_T) {

				uint32_t startLine = whileToken->line+1;
				uint32_t endLine = condition->endLine;

				string message = "Expected bool expression for while condition, not a ";
				message.append(toStringParseDataType(condition->evalType));
				message.append(" one");

        throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), copyString(message.c_str()));
      }
      
      AbstractStatementNode* body = addStatement();
      return new WhileStatementNode(condition, body, symbolTable, whileToken->line+1);
    }
    
    case FOR: {
      
      Token* forToken = consume(); //consume for Token
      Token* leftParenToken = consume();

			//enter new scope for static checking purposes
			symbolTable->enterNewScope();

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
			Token* semiColonToken = consume();
      
			//if semicolon not found, throw error
      if(semiColonToken->type != SEMICOLON) {

				uint32_t startLine = forToken->line+1;
				uint32_t endLine = (semiColonToken->type == END) ? initialization->endLine : semiColonToken->line+1;

				if(semiColonToken->type == END) {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected ';' in 'for' loop syntax");
				} else {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), semiColonToken->lexeme, "Expected ';' in 'for' loop syntax");
				}
      }
      
      AbstractExpressionNode* condition = evalExpression();
      if(condition->evalType != BOOL_T) {
				
        uint32_t startLine = forToken->line+1;
				uint32_t endLine = condition->endLine;

				string message = "Expected bool expression for 'for' termination condition, not a ";
				message.append(toStringParseDataType(condition->evalType));
				message.append(" one");

				throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), copyString(message.c_str()));
      }
      
			//syntax error if ; not found
			Token* semicolonToken = consume();
      if(semicolonToken->type != SEMICOLON) {

				uint32_t startLine = forToken->line+1;
				uint32_t endLine = (semicolonToken->type == END) ? condition->endLine : semicolonToken->line+1;

				if(semicolonToken->type == END) {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected ';' after termination condition in 'for' loop");
				} else {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), semicolonToken->lexeme, "Expected ';' after termination condition in 'for' loop");
				}
      }
      
      AbstractStatementNode* update = addStatement();
			Token* rightParenToken = consume();

			//if next token is not ')', throw error
      if(rightParenToken->type != RIGHT_PAREN) {

        uint32_t startLine = forToken->line+1;
				uint32_t endLine = (rightParenToken->type == END) ? update->endLine : rightParenToken->line+1;

				if(rightParenToken->type == END) {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected ';' after 'for' loop update condition");
				} else {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), rightParenToken->lexeme, "Expected ';' after 'for' loop update condition");
				}
      }

      AbstractStatementNode* body = addStatement();

			//leave scope
			symbolTable->leaveScope();
			
      return new ForStatementNode(initialization, update, body, condition, symbolTable, forToken->line+1);
    }
    
    case IF: {
    
      vector<AbstractExpressionNode*>* cond = new vector<AbstractExpressionNode*>();
      vector<AbstractStatementNode*>* stat = new vector<AbstractStatementNode*>();
			uint32_t currentEndLine;
      
      //consume the IF token and add first condition-statement pair
      Token* ifToken = consume();
      AbstractExpressionNode* e = evalExpression();
      
			//make sure the condition is a boolean
      if(e->evalType != BOOL_T) {
        
				uint32_t startLine = ifToken->line+1;
				uint32_t endLine = e->endLine;

				string message = "'if' condition requires bool expression, not a ";
				message.append(toStringParseDataType(e->evalType));
				message.append(" one");

				throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), copyString(message.c_str()));

      } else {
      	cond->push_back(e);
				AbstractStatementNode* currStatement = addStatement();
				currentEndLine = currStatement->endLine;
      	stat->push_back(currStatement);
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
						uint32_t endLine = exp->endLine;

						string message = "'elif' condition requires a bool expression, not a ";
						message.append(toStringParseDataType(exp->evalType));
						message.append(" one");

            throw StaticTypeError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), copyString(message.c_str()));

          } else {
            cond->push_back(exp);
						AbstractStatementNode* currStatement = addStatement();
						currentEndLine = currStatement->endLine;
            stat->push_back(currStatement);
          }
           
        } else {
          
          //consume ELSE token and add dummy true-statement pair
          Token* elseToken = consume();
          
          ParseData d;
          d.type = BOOL_T; d.value.integer = true;
          LiteralNode* exp = new LiteralNode(d, elseToken->line+1);
          
          cond->push_back(exp);
					AbstractStatementNode* currStatement = addStatement();
					currentEndLine = currStatement->endLine;
          stat->push_back(currStatement);
          
          return new ConditionalStatementNode(cond, stat, symbolTable, ifToken->line+1, currentEndLine);
        }  
      }
      
      return new ConditionalStatementNode(cond, stat, symbolTable, ifToken->line+1, currentEndLine);
    }

		case RETURN: {

			Token* returnToken = consume(); //consume return
			AbstractExpressionNode* returnExpression = evalExpression();

			//make sure we're even inside a function
			if(returnType.size() < 1) {
				uint32_t startLine = returnToken->line+1;
				uint32_t endLine = returnExpression->endLine;
				throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "return", "There is no enclosing function to return out of.");
			}

			//make sure return type is valid
			if(!typecheckImplicitCastExpression(returnExpression->evalType, returnType.back())) {
				uint32_t startLine = returnToken->line+1;
				uint32_t endLine = returnExpression->endLine;
				throw StaticCastError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), returnExpression->evalType, returnType.back(), false);
			}

			return new ReturnStatementNode(returnExpression, returnFlag.back(), returnValue.back(), returnToken->line+1);
		}

		case FUN: {

			Token* funToken = consume(); //consume the FUN token
			Token* varToken = consume(); //function name

			//next token should be a variable, the function name
			if(varToken->type != VARIABLE) {

				uint32_t startLine = funToken->line+1;
				uint32_t endLine = (varToken->type == END) ? startLine : varToken->line+1;

				if(varToken->type == END) {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), "Expected function name after 'fun' declaration");
				} else {
					throw ParseSyntaxError(startLine, endLine, getCodeLineBlock(startLine-1, endLine-1), varToken->lexeme, "Expected function name after 'fun' declaration");
				} 
			}

			//parse function and its arguments
			string functionName(varToken->lexeme);
			Function* function = parseFunction(funToken->line+1, varToken->line+1, functionName);

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
	returnType = vector<ParseDataType>();
	insideClassDefinition = false;
  
  //create empty statement vector
  vector<AbstractStatementNode*>* statements = new vector<AbstractStatementNode*>();
  
  //append statement nodes until END is reached
  while(peek()->type != END) {
    statements->push_back(addStatement());
  }
  
  return statements;
}


