#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <cstring>
#include <vector>
#include <unordered_map>
#include "parsetoken.h"


class SymbolTable {
  
  private:
    std::vector<std::unordered_map<char*, ParseData>*>* table;
 
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
