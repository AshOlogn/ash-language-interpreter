#include <iostream>
#include <fstream>
#include <cstdint>
#include <cctype>

#include "errors.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "parsenode.h"
#include "parsetoken.h"
#include "evaluator.h"
#include "statementnode.h"
#include "symboltable.h"
#include "function.h"

using namespace std;


vector<char*>* getLines(char* code);

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

  //convert source code into array of lines
  vector<char*>* codeLines = getLines(sourceCode);
  
  //generate array of Tokens from source code 
  vector<Token> tokens;
  
  try {
		cout << "LEXING STAGE:" << endl;
    tokens = lex(sourceCode, codeLines);
  } catch(exception& e) {
    cout << e.what() << endl;
    return 1;
  }
  
	vector<Token>::iterator it;
	for(it = tokens.begin(); it != tokens.end(); it++) {
		cout << toStringTokenType(it->type) << " " << it->line << endl;
	}
	
  //get list of statements from list of tokens (parse)
  vector<AbstractStatementNode*>* statements;
  
  try {
		cout << "PARSING STAGE" << endl;
    statements = parse(&tokens, codeLines); 
  } catch(exception& e) {
    cout << e.what() << endl;
    return 1;
  }
  
  vector<AbstractStatementNode*>::iterator it2;
	
  //execute statements
	cout << "EXECUTION STAGE" << endl;
  for(it2 = statements->begin(); it2 != statements->end(); it2++) {

		try {
			(*it2)->execute();
		}	catch(exception& e) {
			cout << e.what() << endl;
			return 1;
		}	
  }

}

//convert source code into array of lines
vector<char*>* getLines(char* code) {
  
  vector<char*>* lines = new vector<char*>();
  uint32_t codeIndex = 0;
  
  while(code[codeIndex]) {
    
    //edge cases
    if(code[codeIndex] == '\n') {
      char* c = new char[2];
      c[0] = '\n'; c[1] = '\0';
      lines->push_back(c);
      codeIndex++;
      continue;
    }
    
    //setup
    uint32_t start = codeIndex;
    uint32_t end = codeIndex;
    
    while(code[end] != '\n' && code[end] != '\0') {
      end++;
    }
    
    char* c = new char[end-start+2];
    for(uint32_t i = start; i < end; i++) {
      c[i-start] = code[i];
    }
    c[end-start] = '\n';
    c[end-start+1] = '\0';

    lines->push_back(c);
    codeIndex = end+1;
  }
  
  return lines;
}

