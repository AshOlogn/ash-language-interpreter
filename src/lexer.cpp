#include <iostream>
#include <cstring>
#include <cstdint>
#include <cctype>
#include <vector>

#include "exceptions.h"
#include "token.h"
#include "lexer.h"

using namespace std;

static vector<char*>* codeLines;
static uint32_t line = 0;

//Convert string to uint64_t
uint64_t stringToInt(char* str, uint32_t start, uint32_t end) {

  uint64_t ans = 0;
  for(uint32_t i = start; i <= end; i++) {
    ans = ans*10 + (str[i] - '0');
  }  

  return ans;
}

//Convert string to double
double stringToDouble(char* str, uint32_t start, uint32_t end) {

  double whole = 0;
  double decimal = 0;
  double exp = 0.1;
  bool isDecimalMode = false;

  for(uint32_t i = start; i <= end; i++) {

    if(str[i] == '.') {
      isDecimalMode = true;
    } else if(isDecimalMode) {
      decimal += exp*(str[i]-'0');
      exp *= 0.1;      
    } else {
      whole = whole*10 + (str[i]-'0');      
    }
  }

  return whole + decimal;
}


//Lexes source code into array of Tokens
vector<Token> lex(char* code, vector<char*>* sourceCodeLines) {
  
  codeLines = sourceCodeLines;
  
  //store code length
  uint32_t codeLength = strlen(code);
 
  vector<Token> tokens;
  uint32_t index = 0;
  line = 0;  

  while(code[index] != 0) {

    //first parse fixed-size tokens
    switch(code[index]) {
  
      case '(': {
        tokens.push_back(makeToken(LEFT_PAREN, line, (char*) "("));
        index++; break;
      }  

      case ')': {
        tokens.push_back(makeToken(RIGHT_PAREN, line, (char*) ")"));
        index++; break;
      }

      case '[': {
        tokens.push_back(makeToken(LEFT_BRACKET, line, (char*) "["));
        index++; break;
      }  
  
      case ']': {
        tokens.push_back(makeToken(RIGHT_BRACKET, line,  (char*) "]"));
        index++; break;
      }  

      case '{': {
        tokens.push_back(makeToken(LEFT_BRACE, line, (char*) "{"));
        index++; break;
      }  

      case '}': {
        tokens.push_back(makeToken(RIGHT_BRACE, line, (char*) "}"));
        index++; break;
      }  

      case '.': {
        if(!isdigit(code[index+1])) {
          tokens.push_back(makeToken(PERIOD, line, (char*) "."));
          index++; 
        }
        break;
      }

      case '?': {
        tokens.push_back(makeToken(QUESTION, line, (char*) "?"));
        index++; break;
      }

      case ':': {
        tokens.push_back(makeToken(COLON, line, (char*) ":"));
        index++; break;
      }

      case ';': {
        tokens.push_back(makeToken(SEMICOLON, line, (char*) ";"));
        index++; break;
      }

      case ',': {
        tokens.push_back(makeToken(COMMA, line, (char*) ","));
        index++; break;
      }

      //more complex fixed-length tokens
      case '+': {
  
        if(code[index+1]) {
          
          switch(code[index+1]) {
  
            case '+': {
              tokens.push_back(makeToken(INC, line, (char*) "++"));
              index += 2; break;
            }

            case '=': {
              tokens.push_back(makeToken(ADD_EQ, line, (char*) "+="));
              index += 2; break;
            }
    
            default: {
              tokens.push_back(makeToken(ADD, line, (char*) "+"));
              index++; break;
            }
          }

        } else {
          tokens.push_back(makeToken(ADD, line, (char*) "+"));
          index++;
        }

        break;
      }

      case '-': {
  
        if(code[index+1]) {
          
          switch(code[index+1]) {
  
            case '-': {
              tokens.push_back(makeToken(DEC, line, (char*) "--"));
              index += 2; break;
            }

            case '=': {
              tokens.push_back(makeToken(SUBTRACT_EQ, line, (char*) "-="));
              index += 2; break;
            }
    
            default: {
              tokens.push_back(makeToken(SUBTRACT, line, (char*) "-"));
              index++; break;
            }
          }

        } else {
          tokens.push_back(makeToken(SUBTRACT, line, (char*)  "-"));
          index++;
        }

        break;
      }

      case '*': {
  
        if(code[index+1] == '*') {
          
          if(code[index+2] == '=') {
            tokens.push_back(makeToken(EXPONENT_EQ, line, (char*) "**="));
            index+=3; 
          } else { 
            tokens.push_back(makeToken(EXPONENT, line, (char*) "**"));
            index+=2;
          }          

        } else if(code[index+1] == '=') {
          tokens.push_back(makeToken(MULTIPLY_EQ, line, (char*) "*="));
          index+=2;
        } else {
          tokens.push_back(makeToken(MULTIPLY, line, (char*) "*"));
          index++;
        }

        break;
      }

      case '/': {
 
        if(code[index+1] == '/') {

          //comment out code until end of line
          while(code[index] != '\n' && code[index] != 0) {
            index++;
          }

          if(code[index] != 0)
            index++;

        } else if(code[index+1] == '*') {
  
          uint32_t depth = 1;
          index += 2;
          
          while(depth > 0) {
            if(code[index] == '*' && code[index+1] == '/') {
              depth--;
              index += 2;
            } else if(code[index] == '/' && code[index+1] == '*') {
              depth++;
              index += 2;
            } else {
              index++;
            }
          }   
        
        } else if(code[index+1] == '=') {
          
          tokens.push_back(makeToken(DIVIDE_EQ, line, (char*) "/="));
          index += 2;

        } else {
          tokens.push_back(makeToken(DIVIDE, line, (char*) "/"));
          index++;
        }

        break;
      }

      case '%': {
  
        if(code[index+1] == '=') {

          tokens.push_back(makeToken(MOD_EQ, line, (char*) "%="));
          index += 2;

        } else {
          tokens.push_back(makeToken(MOD, line, (char*) "%"));
          index++;
        }
        
        break;
      }

      case '!': {
  
        if(code[index+1] == '=') {
  
          tokens.push_back(makeToken(NOT_EQ, line, (char*) "!="));
          index += 2;

        } else {
          tokens.push_back(makeToken(NOT, line, (char*) "!"));
          index++;
        }

        break;
      }
  
      case '~': {
        tokens.push_back(makeToken(BIT_NOT, line, (char*) "~"));
        index++; break;
      }

      case '<': {
  
        if(code[index+1] == '<') {
  
          if(code[index+2] == '=') {
            tokens.push_back(makeToken(BIT_LEFT_EQ, line, (char*) "<<="));
            index += 3;
          } else {
            tokens.push_back(makeToken(BIT_LEFT, line, (char*) "<<"));
            index += 2;
          }
         
        } else if(code[index+1] == '=') {
          tokens.push_back(makeToken(LESS_EQ, line, (char*) "<="));
          index += 2;

        } else {
          tokens.push_back(makeToken(LESS, line, (char*) "<"));
          index++;
        }

        break;
      }

      case '>': {
  
        if(code[index+1] == '>') {
  
          if(code[index+2] == '=') {
            tokens.push_back(makeToken(BIT_RIGHT_EQ, line, (char*) ">>="));
            index += 3;
          } else {
            tokens.push_back(makeToken(BIT_RIGHT, line, (char*) ">>"));
            index += 2;
          }
         
        } else if(code[index+1] == '=') {
          tokens.push_back(makeToken(GREATER_EQ, line, (char*) ">="));
          index += 2;
        } else {
          tokens.push_back(makeToken(GREATER, line, (char*) ">"));
          index++;
        }

        break;
      }

      case '=': {
        if(code[index+1] == '=') {
          tokens.push_back(makeToken(EQ_EQ, line, (char*) "=="));
          index += 2;
        } else {
          tokens.push_back(makeToken(EQ, line, (char*) "="));
          index++;
        }
        break;  
      }

      case '&': {
        if(code[index+1] == '&') {
          tokens.push_back(makeToken(AND, line, (char*) "&&"));
          index += 2;
        } else if(code[index+1] == '=') {
          tokens.push_back(makeToken(AND_EQ, line, (char*) "&="));
          index += 2;
        } else {
          tokens.push_back(makeToken(BIT_AND, line, (char*) "&"));
          index++;
        }
        break;
      } 

      case '^': {
        if(code[index+1] == '^') {
          tokens.push_back(makeToken(XOR, line, (char*) "^^"));
          index += 2;
        } else if(code[index+1] == '=') {
          tokens.push_back(makeToken(XOR_EQ, line, (char*) "^="));
          index += 2;
        } else {
          tokens.push_back(makeToken(BIT_XOR, line, (char*) "^"));
          index++;
        }
        break;
      } 

      case '|': {
        if(code[index+1] == '|') {
          tokens.push_back(makeToken(OR, line, (char*) "||"));
          index += 2;
        } else if(code[index+1] == '=') {
          tokens.push_back(makeToken(OR_EQ, line, (char*) "|="));
          index += 2;
        } else {
          tokens.push_back(makeToken(BIT_OR, line, (char*) "|"));
          index++;
        }
        break;
      } 
    }

    //if it's in quotes, it's a String
    if(code[index] == '"') {

      uint32_t currentIndex = index+1;
      uint32_t startLine = line;
      
      while(code[currentIndex] != '"' && currentIndex < codeLength) {

        if(code[currentIndex] == '\n') {
          line++;
        }

        currentIndex++;
      }
      
      
      //allocate memory for the lexeme and copy it
      char* lexeme = (char*) malloc(sizeof(char) * (currentIndex-index));
      for(uint32_t i = index+1; i < currentIndex; i++)
        lexeme[i-index-1] = code[i];
      lexeme[currentIndex-index-1] = 0;
      
      //if closing quotation mark is not found, throw an error
      if(currentIndex == codeLength) {
        // !!!
        throw LexerException(startLine+1, codeLines->at(startLine), lexeme, "String literal not terminated with \"");         
      }


      Data tokenVal;
      char* c = new char[currentIndex-index];
      std::strcpy(c, lexeme);
      tokenVal.allocated = (void*) c;

      tokens.push_back(makeToken(STRING, startLine, lexeme, tokenVal));
      index = currentIndex+1;

    } else if(isdigit(code[index]) || code[index] == '.') {
 
      //parse integers
      uint32_t decimalCount = 0;
      uint32_t currentIndex = index;
    
      while(isdigit(code[currentIndex]) || code[currentIndex] == '.' || code[currentIndex] == '_') {

        if(code[currentIndex] == '.') {
          if(decimalCount > 0) {
            
            // !!!
            //first create lexeme
            char* errorLex = new char[currentIndex-index+2];
            for(uint32_t i = index; i <= currentIndex; i++) {
              errorLex[i-index] = code[i];
            }
            errorLex[currentIndex-index+1] = '\0';   
            throw LexerException(line+1, codeLines->at(line), errorLex, "Number literal can have at most 1 decimal point"); 
            
          } else {
            decimalCount++;
          }
        }
        
        currentIndex++;
      }

      
      //make sure this token actually ends here
      if(isEndOfToken(code[currentIndex])) {
 
        //Copy numerical value to add to Token
        char* lexeme = (char*) malloc(sizeof(char) * (currentIndex-index+1));
        strncpy(lexeme, code+index, currentIndex-index); 
        lexeme[currentIndex-index] = 0;
        
        //union to store parsed numbers
        Data tokenVal;

        if(decimalCount == 1) {
          tokenVal.floatingPoint = stringToDouble(code, index, currentIndex-1);
          tokens.push_back(makeToken(DOUBLE, line, lexeme, tokenVal)); 
        } else {
          tokenVal.integer = stringToInt(code, index, currentIndex-1);
          tokens.push_back(makeToken(INT32, line, lexeme, tokenVal)); 
        }
        
      } else {
        
       // !!! Otherwise throw an exception
       //first copy error lexeme, including bad character
       char* errorLexeme = new char[currentIndex-index+2];
       strncpy(errorLexeme, code+index, currentIndex-index+1);
       errorLexeme[currentIndex-index+1] = '\0';
       
       throw LexerException(line+1, codeLines->at(line), errorLexeme, "Number literal can only contain digits and <= 1 decimal point"); 
      }

      index = currentIndex;

    } else if(code[index] == '_' || isalpha(code[index])) {
      
      uint32_t currentIndex = index;
      while(code[currentIndex] == '_' || isalpha(code[currentIndex]) || isdigit(code[currentIndex])) {
        currentIndex++;
      }

      //if identifier contains invalid characters, throw an error
      if(!isspace(code[currentIndex]) && !isEndOfToken(code[currentIndex])) {
        
        char* errorLexeme = (char*) malloc(sizeof(char) * (currentIndex-index+2));
        strncpy(errorLexeme, code+index, currentIndex-index+1);
        errorLexeme[currentIndex-index+1] = 0;
        
        throw LexerException(line+1, codeLines->at(line), errorLexeme, "Identifier can only contain digits, letters, and underscores");
      } 
      
      char* lexeme = (char*) malloc(sizeof(char) * (currentIndex-index+1));
      strncpy(lexeme, code+index, currentIndex-index);
      lexeme[currentIndex-index] = 0;

      //determine if keyword, if not then identifier
      TokenType tt = varOrKeywordTokenType(lexeme);
      if(tt == TRUE) {
        
        Data d; d.integer = 1;
        tokens.push_back(makeToken(BOOL, line, lexeme, d));
      
      } else if(tt == FALSE) {
        
        Data d; d.integer = 0;
        tokens.push_back(makeToken(BOOL, line, lexeme, d));
        
      } else {
        tokens.push_back(makeToken(varOrKeywordTokenType(lexeme), line, lexeme));  
      }
      
      index = currentIndex;
    }
    
    //consume white space
    while(isspace(code[index])) {
      if(code[index] == '\n') {
        line++;  
      }
      index++;
    }
  }
    
  tokens.push_back(makeToken(END, line, (char*) "END"));

  return tokens;
}
