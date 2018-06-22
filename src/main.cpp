#include <iostream>
#include <fstream>
#include <cstdint>
#include <cctype>

#include "exceptions.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "parsenode.h"
#include "parsetoken.h"
#include "evaluator.h"
#include "statementnode.h"
#include "symboltable.h"

using namespace std;

int main(int argc, char** argv) {
  
  //read in source code
  char* sourceFile = argv[1];
  fstream in(sourceFile, fstream::in);

  in.seekg(0, in.end);
  uint32_t length = in.tellg();
  in.seekg(0, in.beg);
  
  char* sourceCode = new char[length+1];
  in.read(sourceCode, length);
  sourceCode[length] = 0;
  in.close();

  //generate array of Tokens from source code 
  vector<Token> tokens;
  
  try {
    tokens = lex(sourceCode);
  } catch(exception& e) {
    cout << e.what() << endl;
    return 1;
  }
  
  vector<Token>::iterator it;

  for(it = tokens.begin(); it != tokens.end(); it++) {
    cout << toStringTokenType(it->type) << endl;
  }
  
  //get list of statements from list of tokens (parse)
  vector<AbstractStatementNode*>* statements = parse(&tokens);
  vector<AbstractStatementNode*>::iterator it2;
  
	//print string representation of each expression
	/*for(it2 = statements->begin(); it2 != statements->end(); it2++) {
    cout << ((ExpressionStatementNode*) (*it2))->expression->toString() << endl;
  }*/
	
  //execute statements
  for(it2 = statements->begin(); it2 != statements->end(); it2++) {
    (*it2)->execute();
  }

}

