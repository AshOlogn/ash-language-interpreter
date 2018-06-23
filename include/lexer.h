#ifndef LEXER_H
#define LEXER_H

#include <cstdint>
#include <vector>
#include "token.h"


//convert literals to actual numerical values
uint64_t stringToInt(char* str);
double stringToDouble(char* str);

//generates list of Tokens from Ash source code
std::vector<Token> lex(char* code, std::vector<char*>* codeLines);

#endif
