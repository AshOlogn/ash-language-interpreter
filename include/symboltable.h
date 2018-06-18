#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <cstring>
#include <vector>
#include <unordered_map>
#include "parsetoken.h"


struct HashFunction {
  
  std::size_t operator()(char* str) const {
    
    uint32_t hash = 0;
    uint32_t len = strlen(str);
   
    for(uint32_t i = 0; i < len; ++str, ++i) {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
    }

    return (std::size_t) hash;
  } 
};

struct EqFunction {
  
  bool operator()(char* c1, char* c2) const {
    return strcmp(c1, c2) == 0;
  }
};

class SymbolTable {
  
  private:
    std::vector<std::unordered_map<char*, ParseData, HashFunction, EqFunction>*>* table;
 
  public:
    SymbolTable();
    void enterNewScope();
    void leaveScope();
    void declare(char* var, ParseData value);
    void update(char* var, ParseData value);
    bool isDeclaredInScope(char* var);
    bool isDeclared(char* var);
    ParseData get(char* var);
};


#endif
