#include <iostream>
#include <fstream>
#include <cstdint>
#include <cctype>
#include "lexer.h"

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
  vector<Token*> tokens = lex(sourceCode);  

  for(Token* t : tokens) {
    cout << toStringTokenType(t->type) << endl;
  }
  
}
