#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <iostream>
#include "token.h"

//lexer error reporting
void lexError(const char* message);

//convert literals to actual numerical values
uint64_t stringToInt(char* str);
double stringToDouble(char* str);

//generates list of Tokens from Ash source code
std::vector<Token*> lex(char* code);

#endif