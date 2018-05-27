#include <iostream>
#include <cstring>
#include <cstdint>
#include <cctype>
#include "lexer.h"

using namespace std;

const char* RESERVED_WORDS[] = {"for", "while", "do", "if", "else", "break",
                               "switch", "case", "class", "fun", "int8",
                              "int16", "int32", "int64", "uint8", "uint16",
                              "uint32", "uint64", "char", "double", "bool",
                              "string"};

TokenType RESERVED_WORD_TOKENS[] = { FOR, WHILE, DO, IF, ELSE, BREAK,
                                    SWITCH, CASE, CLASS, FUN, INT8,
                                    INT16, INT32, INT64, UINT8, UINT16,
                                    UINT32, UINT64, CHAR, DOUBLE, BOOL,
                                    STRING };

const uint8_t NUM_RESERVED_WORDS =  22;


//determines whether the token is a type keyword
bool isTypeToken(TokenType token_type) {
  
  return token_type == INT64 || token_type == CHAR || 
         token_type == DOUBLE || token_type == BOOL || 
         token_type == STRING;
}

//Token full constructor
Token::Token(TokenType typ, uint32_t lin, char* lex, TokenValue val) {
  type = typ; line = lin; lexeme = lex; value = val;
}

//Token constructor for tokens without "value" 
Token::Token(TokenType typ, uint32_t lin, char* lex) {
  type = typ; line = lin; lexeme = lex;
}

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


bool isSingleCharToken(char c) {  
  
  return c == '+' || c == '-' || c == '*' || c == '/' ||
         c == '%' || c == '&' || c == '|' || c == '^' ||
         c == '!' || c == '=' || c == '<' || c == '>' ||
         c == '?' || c == ':' || c == '.' || c == ',' || 
         c == ';' || c == '(' || c == ')' || c == '[' || 
         c == ']' || c == '{' || c == '}';
}


bool isEndOfToken(char c) {
  return c == 0 || isspace(c) || isSingleCharToken(c);
}


//Lexes source code into array of Tokens
vector<Token> lex(char* code) {

  vector<Token> tokens;
  uint32_t index = 0;
  uint32_t line = 1;  

  while(code[index]) {
  
    //first parse fixed-size tokens
    switch(code[index]) {
  
      case '(': {
        tokens.push_back(Token(LEFT_PAREN, line, (char*) "("));
        index++; break;
      }  

      case ')': {
        tokens.push_back(Token(RIGHT_PAREN, line, (char*) ")"));
        index++; break;
      }

      case '[': {
        tokens.push_back(Token(LEFT_BRACKET, line, (char*) "["));
        index++; break;
      }  
  
      case ']': {
        tokens.push_back(Token(RIGHT_BRACKET, line, (char*) "]"));
        index++; break;
      }  

      case '{': {
        tokens.push_back(Token(LEFT_BRACE, line, (char*) "{"));
        index++; break;
      }  

      case '}': {
        tokens.push_back(Token(RIGHT_BRACE, line, (char*) "}"));
        index++; break;
      }  

      case '.': {
        tokens.push_back(Token(PERIOD, line, (char*) "."));
        index++; break;
      }

      case '?': {
        tokens.push_back(Token(QUESTION, line, (char*) "?"));
        index++; break;
      }

      case ':': {
        tokens.push_back(Token(COLON, line, (char*) ":"));
        index++; break;
      }

      case ';': {
        tokens.push_back(Token(SEMICOLON, line, (char*) ";"));
        index++; break;
      }

      case ',': {
        tokens.push_back(Token(COMMA, line, (char*) ","));
        index++; break;
      }

      //more complex fixed-length tokens
      case '+': {
  
        if(code[index+1]) {
          
          switch(code[index+1]) {
  
            case '+': {
              tokens.push_back(Token(INC, line, (char*) "++"));
              index += 2; break;
            }

            case '=': {
              tokens.push_back(Token(ADD_EQ, line, (char*) "+="));
              index += 2; break;
            }
    
            default: {
              tokens.push_back(Token(ADD, line, (char*) "+"));
              index++; break;
            }
          }

        } else {
          tokens.push_back(Token(ADD, line, (char*) "+"));
          index++;
        }

        break;
      }

      case '-': {
  
        if(code[index+1]) {
          
          switch(code[index+1]) {
  
            case '-': {
              tokens.push_back(Token(DEC, line, (char*) "--"));
              index += 2; break;
            }

            case '=': {
              tokens.push_back(Token(SUBTRACT_EQ, line, (char*) "-="));
              index += 2; break;
            }
    
            default: {
              tokens.push_back(Token(SUBTRACT, line, (char*) "-"));
              index++; break;
            }
          }

        } else {
          tokens.push_back(Token(SUBTRACT, line, (char*) "-"));
          index++;
        }

        break;
      }


      case '*': {
  
        if(code[index+1] == '*') {
          
          if(code[index+2] == '=') {
            tokens.push_back(Token(EXPONENT_EQ, line, (char*) "**="));
            index+=3; 
          } else { 
            tokens.push_back(Token(EXPONENT, line, (char*) "**"));
            index+=2;
          }          

        } else if(code[index+1] == '=') {
          tokens.push_back(Token(MULTIPLY_EQ, line, (char*) "*="));
          index+=2;
        } else {
          tokens.push_back(Token(MULTIPLY, line, (char*) "*"));
          index++;
        }

        break;
      }

      case '/': {
  
        if(code[index+1] == '=') {
          
          tokens.push_back(Token(DIVIDE_EQ, line, (char*) "/="));
          index += 2;

        } else {
          tokens.push_back(Token(DIVIDE, line, (char*) "/"));
          index++;
        }

        break;
      }

      case '%': {
  
        if(code[index+1] == '=') {

          tokens.push_back(Token(MOD_EQ, line, (char*) "%="));
          index += 2;

        } else {
          tokens.push_back(Token(MOD, line, (char*) "%"));
          index++;
        }
        
        break;
      }

      case '!': {
  
        if(code[index+1] == '=') {
  
          tokens.push_back(Token(NOT_EQ, line, (char*) "!="));
          index += 2;

        } else {
          tokens.push_back(Token(BIT_NOT, line, (char*) "!"));
          index++;
        }

        break;
      }

      case '<': {
  
        if(code[index+1] == '<') {
  
          if(code[index+2] == '=') {
            tokens.push_back(Token(BIT_LEFT_EQ, line, (char*) "<<="));
            index += 3;
          } else {
            tokens.push_back(Token(BIT_LEFT, line, (char*) "<<"));
            index += 2;
          }
         
        } else if(code[index+1] == '=') {
          tokens.push_back(Token(LESS_EQ, line, (char*) "<="));
          index += 2;

        } else {
          tokens.push_back(Token(LESS, line, (char*) "<"));
          index++;
        }

        break;
      }

      case '>': {
  
        if(code[index+1] == '>') {
  
          if(code[index+2] == '=') {
            tokens.push_back(Token(BIT_RIGHT_EQ, line, (char*) ">>="));
            index += 3;
          } else {
            tokens.push_back(Token(BIT_RIGHT, line, (char*) ">>"));
            index += 2;
          }
         
        } else if(code[index+1] == '=') {
          tokens.push_back(Token(GREATER_EQ, line, (char*) ">="));
          index += 2;
        } else {
          tokens.push_back(Token(GREATER, line, (char*) ">"));
          index++;
        }

        break;
      }

      case '=': {
        if(code[index+1] == '=') {
          tokens.push_back(Token(EQ_EQ, line, (char*) "=="));
          index += 2;
        } else {
          tokens.push_back(Token(EQ, line, (char*) "="));
          index++;
        }
        break;  
      }

      case '&': {
        if(code[index+1] == '=') {
          tokens.push_back(Token(AND_EQ, line, (char*) "&="));
          index += 2;
        } else {
          tokens.push_back(Token(BIT_AND, line, (char*) "&"));
          index++;
        }
        break;
      } 

      case '^': {
        if(code[index+1] == '=') {
          tokens.push_back(Token(XOR_EQ, line, (char*) "^="));
          index += 2;
        } else {
          tokens.push_back(Token(BIT_XOR, line, (char*) "^"));
          index++;
        }
        break;
      } 

      case '|': {
        if(code[index+1] == '=') {
          tokens.push_back(Token(OR_EQ, line, (char*) "|="));
          index += 2;
        } else {
          tokens.push_back(Token(BIT_OR, line, (char*) "|"));
          index++;
        }
        break;
      } 
    }

    //if it's in quotes, it's a String
    if(code[index] == '"') {
    
      uint32_t currentIndex = index+1;
      while(currentIndex != '"') {
        if(currentIndex == '\n')
          line++;
        currentIndex++;
      }
      
      //allocate memory for the lexeme and copy it
      char* lexeme = (char*) malloc(sizeof(char) * (currentIndex-index));
      for(uint32_t i = index+1; i < currentIndex; i++)
        lexeme[i-index-1] = code[i];
      lexeme[currentIndex-index-1] = 0;

      tokens.push_back(Token(STRING, line, lexeme));
      index = currentIndex+1;

    } else if(isdigit(code[index]) || code[index] == '.') {
  
      //parse integers
      uint32_t decimalCount = 0;
      uint32_t currentIndex = index;
      while(isdigit(code[currentIndex] || code[currentIndex] == '.' || code[currentIndex] == '_')) {

        if(code[currentIndex] == '.') {
          if(decimalCount > 0) {
            cout << "Error: number literal can have at most 1 decimal point" << endl;
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
        TokenValue tokenVal;

        if(decimalCount == 1) {
          tokenVal.floatingPoint = stringToDouble(code, index, currentIndex-1);
          tokens.push_back(Token(DOUBLE, line, lexeme, tokenVal)); 
        } else {
          tokenVal.integer = stringToInt(code, index, currentIndex-1);
          tokens.push_back(Token(UINT64, line, lexeme, tokenVal)); 
        }
      }

      index = currentIndex;

    } else if(code[index] == '_' || isalpha(code[index])) {
      
      uint32_t currentIndex = index;
      while(code[index] == '_' || isalpha(code[currentIndex]) || isdigit(code[currentIndex])) {
        currentIndex++;
      }

      char* lexeme = (char*) malloc(sizeof(char) * (currentIndex-index+1));
      strncpy(lexeme, code+index, currentIndex-index);
      lexeme[currentIndex-index] = 0;

      //determine if keyword, if not then identifier
      uint32_t len = currentIndex-index;
      bool isReserved = false;

      for(uint8_t i = 0; i < NUM_RESERVED_WORDS; i++) {
        if(strncmp(lexeme, RESERVED_WORDS[i], len)) {
          isReserved = true;
          tokens.push_back(Token(RESERVED_WORD_TOKENS[i], line, lexeme));                    
        }
      }
      
      if(!isReserved) {
        tokens.push_back(Token(VARIABLE, line, lexeme));
      }

      index = currentIndex;
    }

    //consume white space
    while(isspace(code[index])) {
      if(code[index] == '\n')
        line++;
      index++;
    }

  }


  return tokens;
}
